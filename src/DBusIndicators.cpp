// -*- Mode: C++; indent-tabs-mode: nil; tab-width: 2 -*-
/*
 * Copyright (C) 2010 Canonical Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Neil Jagdish Patel <neil.patel@canonical.com>
 */

#include "DBusIndicators.h"

#include <algorithm>
#include <iostream>

#include <X11/Xlib.h>
#include "Nux/Nux.h"
#include "Nux/WindowThread.h"
#include "NuxGraphics/GLWindowManager.h"

#include "config.h"
#include "Variant.h"

using std::cout;
using std::endl;

namespace unity {
namespace indicator {

const char* const S_NAME = "com.canonical.Unity.Panel.Service";
const char* const S_PATH = "/com/canonical/Unity/Panel/Service";
const char* const S_IFACE = "com.canonical.Unity.Panel.Service";

struct SyncData
{
  SyncData(DBusIndicators* self_)
  : self(self_)
  , cancel(g_cancellable_new())
  {
  }

  ~SyncData()
  {
    if (cancel) {
      g_cancellable_cancel(cancel);
      g_object_unref(cancel);
    }
  }

  void SyncComplete()
  {
    if (cancel) {
     g_object_unref(cancel);
    }
    cancel = NULL;
  }

  DBusIndicators* self;
  GCancellable* cancel;
};

namespace {
// This anonymous namespace holds the DBus callback methods.

struct ShowEntryData
{
  GDBusProxy* proxy;
  std::string entry_id;
  int x;
  int y;
  guint timestamp;
  guint32 button;
};

bool run_local_panel_service();
gboolean reconnect_to_service(gpointer data);
void on_proxy_ready_cb(GObject* source, GAsyncResult* res, gpointer data);
void on_proxy_name_owner_changed(GDBusProxy* proxy, GParamSpec* pspec,
                                 DBusIndicators* remote);
void on_proxy_signal_received(GDBusProxy* proxy,
                              char* sender_name, char* signal_name,
                              GVariant* parameters, DBusIndicators* remote);
void request_sync(GDBusProxy* proxy, const char* method, GVariant* name, SyncData* data);
void on_sync_ready_cb(GObject* source, GAsyncResult* res, gpointer data);

bool send_show_entry(ShowEntryData *data);

} // anonymous namespace


// Public Methods
DBusIndicators::DBusIndicators()
: proxy_(NULL)
{
  Reconnect();
}

DBusIndicators::~DBusIndicators()
{
  if (G_IS_OBJECT (proxy_))
  {
    g_signal_handler_disconnect(proxy_, proxy_signal_id_);
    g_signal_handler_disconnect(proxy_, proxy_name_id_);
    g_object_unref(proxy_);
  }
}

void DBusIndicators::Reconnect()
{
  g_spawn_command_line_sync("killall unity-panel-service",
                            NULL, NULL, NULL, NULL);

  if (g_getenv ("PANEL_USE_LOCAL_SERVICE"))
  {
    run_local_panel_service();
    g_timeout_add_seconds(1, reconnect_to_service, this);
  }
  else
  {
    // We want to grab the Panel Service object. This is async, which is fine
    reconnect_to_service(this);
  }
}

void DBusIndicators::OnRemoteProxyReady(GDBusProxy *proxy)
{
  if (proxy_)
  {
    // We've been connected before; We don't need new proxy, just continue
    // rocking with the old one.
    g_object_unref(proxy);
  }
  else
  {
    proxy_ = proxy;
    // Connect to interesting signals
    proxy_signal_id_ = g_signal_connect(proxy_, "g-signal",
                                        G_CALLBACK(on_proxy_signal_received),
                                        this);
    proxy_name_id_ = g_signal_connect(proxy_, "notify::g-name-owner",
                                      G_CALLBACK(on_proxy_name_owner_changed),
                                      this);
  }
  RequestSyncAll();
}

void DBusIndicators::RequestSyncAll()
{
  SyncDataPtr data(new SyncData(this));
  pending_syncs_.push_back(data);
  request_sync(proxy_, "Sync", NULL, data.get());
}

void DBusIndicators::RequestSyncIndicator(std::string const& name)
{
  SyncDataPtr data(new SyncData(this));
  pending_syncs_.push_back(data);
  // The ownership of this variant is taken by the g_dbus_proxy_call.
  GVariant* v_name = g_variant_new("(s)", name.c_str());
  request_sync(proxy_, "SyncOne", v_name, data.get());
}


void DBusIndicators::OnEntryShowMenu(std::string const& entry_id,
                                     int x, int y, int timestamp, int button)
{
  Display* d = nux::GetGraphicsDisplay()->GetX11Display();
  XUngrabPointer(d, CurrentTime);
  XFlush(d);

  // We have to do this because on certain systems X won't have time to
  // respond to our request for XUngrabPointer and this will cause the
  // menu not to show
  ShowEntryData* data = new ShowEntryData();
  data->proxy = proxy_;
  data->entry_id = entry_id;
  data->x = x;
  data->y = y;
  data->timestamp = timestamp;
  data->button = button;

  g_timeout_add(0, (GSourceFunc)send_show_entry, data);

  // --------------------------------------------------------------------------
  // FIXME: This is a workaround until the non-paired events issue is fixed in
  // nux
  XButtonEvent ev = {
    ButtonRelease,
    0,
    False,
    d,
    0,
    0,
    0,
    CurrentTime,
    x, y,
    x, y,
    0,
    Button1,
    True
  };
  XEvent *e = (XEvent*)&ev;
  nux::GetGraphicsThread()->ProcessForeignEvent (e, NULL);
  // --------------------------------------------------------------------------
}

void DBusIndicators::OnEntryScroll(std::string const& entry_id, int delta)
{
  g_dbus_proxy_call(proxy_, "ScrollEntry",
                    g_variant_new("(si)", entry_id.c_str(), delta),
                    G_DBUS_CALL_FLAGS_NONE,
                    -1, NULL, NULL, NULL);
}

void DBusIndicators::Sync(GVariant *args, SyncData* data)
{
  GVariantIter *iter            = NULL;
  gchar        *indicator_id    = NULL;
  gchar        *entry_id        = NULL;
  gchar        *label           = NULL;
  gboolean      label_sensitive = false;
  gboolean      label_visible   = false;
  guint32       image_type      = 0;
  gchar        *image_data      = NULL;
  gboolean      image_sensitive = false;
  gboolean      image_visible   = false;

  // sanity check
  if (!args)
    return;

  std::map<std::string, Indicator::Entries> indicators;
  // We need to make sure they are added in the order they arrive.
  std::vector<std::string> indicator_order;

  g_variant_get (args, "(a(sssbbusbb))", &iter);
  while (g_variant_iter_loop (iter, "(sssbbusbb)",
                              &indicator_id,
                              &entry_id,
                              &label,
                              &label_sensitive,
                              &label_visible,
                              &image_type,
                              &image_data,
                              &image_sensitive,
                              &image_visible))
  {
    // NULL entries (entry_id == "") are just padding
    std::string entry(entry_id);
    if (entry != "")
    {
      indicator_order.push_back(indicator_id);
      Indicator::Entries& entries = indicators[indicator_id];
      Entry::Ptr e(new Entry(entry,
                             label,
                             label_sensitive,
                             label_visible,
                             image_type,
                             image_data,
                             image_sensitive,
                             image_visible));
      entries.push_back(e);
    }
  }
  g_variant_iter_free(iter);

  // Now update each of the entries.
  std::string curr_indicator;
  for (std::vector<std::string>::iterator i = indicator_order.begin(), end = indicator_order.end();
       i != end; ++i)
  {
    std::string const& indicator_name = *i;
    if (curr_indicator != indicator_name) {
      curr_indicator = indicator_name;
      GetIndicator(curr_indicator).Sync(indicators[curr_indicator]);
    }
  }

  // Clean up the SyncData.  NOTE: don't use find when passing in a raw
  // pointer due to explicit construction of the shared pointer.  Could write
  // a predicate, but often a for loop is easier to understand.
  data->SyncComplete();
  for (PendingSyncs::iterator i = pending_syncs_.begin(), end = pending_syncs_.end();
       i != end; ++i)
  {
    if (i->get() == data)
    {
      pending_syncs_.erase(i);
      break;
    }
  }

  // Notify listeners we have new data
  on_synced.emit();
}

void DBusIndicators::SyncGeometries(GVariant* args)
{
  if (proxy_) {
    g_dbus_proxy_call(proxy_, "SyncGeometries", args,
                      G_DBUS_CALL_FLAGS_NONE,
                      -1,
                    NULL,
                      NULL,
                      NULL);
  }
}


void DBusIndicators::AddProperties(GVariantBuilder *builder)
{
  gchar *name = NULL;
  gchar *uname = NULL;

  g_object_get (proxy_,
                "g-name", &name,
                "g-name-owner", &uname,
                NULL);

  bool using_local_service = g_getenv ("PANEL_USE_LOCAL_SERVICE") != NULL;
  variant::BuilderWrapper(builder)
    .add("backend", "remote")
    .add("service-name", name)
    .add("service-unique-name", uname)
    .add("using-local-service", using_local_service);

  g_free (name);
  g_free (uname);
}

GDBusProxy* DBusIndicators::GetRemoteProxy()
{
  return proxy_;
}

namespace {

// Initialise DBus for the panel service, and let us know when it is
// ready.  The unused bool return is to fit with the GSourceFunc.
gboolean reconnect_to_service(gpointer data)
{
  g_dbus_proxy_new_for_bus(G_BUS_TYPE_SESSION,
                           G_DBUS_PROXY_FLAGS_DO_NOT_LOAD_PROPERTIES,
                           NULL,
                           S_NAME,
                           S_PATH,
                           S_IFACE,
                           NULL,
                           on_proxy_ready_cb,
                           data);
  return false;
}

// Make sure the proxy object exists and has a name, and if so, pass
// that on to the DBusIndicators.
void on_proxy_ready_cb(GObject* source, GAsyncResult* res, gpointer data)
{
  DBusIndicators* remote = reinterpret_cast<DBusIndicators*>(data);
  GError* error = NULL;
  GDBusProxy* proxy = g_dbus_proxy_new_for_bus_finish(res, &error);

  static bool force_tried = false;
  char* name_owner = g_dbus_proxy_get_name_owner(proxy);

  if (G_IS_DBUS_PROXY(proxy) && name_owner)
  {
    remote->OnRemoteProxyReady(G_DBUS_PROXY(proxy));
    g_free(name_owner);
    return;
  }
  else
  {
    if (force_tried)
    {
      printf ("\nWARNING: Unable to connect to the unity-panel-service %s\n",
              error ? error->message : "Unknown");
      if (error)
        g_error_free (error);
    }
    else
    {
      force_tried = true;
      run_local_panel_service();
      g_timeout_add_seconds (2, reconnect_to_service, remote);
    }
  }

  g_object_unref (proxy);
}


bool run_local_panel_service()
{
  GError* error = NULL;

  // This is obviously hackish, but this part of the code is mostly hackish...
  // Let's attempt to run it from where we expect it to be
  std::string cmd = PREFIXDIR + std::string("/lib/unity/unity-panel-service");
  std::cerr << "\nWARNING: Couldn't load panel from installed services, "
            << "so trying to load panel from known location: "
            << cmd << "\n";

  g_spawn_command_line_async(cmd.c_str(), &error);
  if (error)
  {
    std::cerr << "\nWARNING: Unable to launch remote service manually: "
              << error->message << "\n";
    g_error_free(error);
    return false;
  }
  return true;
}

void on_proxy_signal_received(GDBusProxy* proxy,
                              char* sender_name, char* signal_name_,
                              GVariant* parameters, DBusIndicators* remote)
{
  std::string signal_name(signal_name_);
  if (signal_name == "EntryActivated")
  {
    const char* entry_name = g_variant_get_string(g_variant_get_child_value(parameters, 0), NULL);
    if (entry_name) {
      remote->ActivateEntry(entry_name);
      cout << "DBusSignal: EntryActivated: \"" << entry_name << "\"" << endl;
    }
    else {
      cout << "DBusSignal: EntryActivated: passed NULL" << endl;
    }
  }
  else if (signal_name == "EntryActivateRequest")
  {
    const char* entry_name = g_variant_get_string(g_variant_get_child_value(parameters, 0), NULL);
    if (entry_name) {
      remote->on_entry_activate_request.emit(entry_name);
      cout << "DBusSignal: EntryActivateRequest: \"" << entry_name << "\"" << endl;
    }
    else {
      cout << "DBusSignal: EntryActivateRequest: passed NULL" << endl;
    }
  }
  else if (signal_name == "ReSync")
  {
    const char* id = g_variant_get_string(g_variant_get_child_value(parameters, 0), NULL);
    bool sync_one = !g_strcmp0 (id, "") == 0;

    if (sync_one)
      remote->RequestSyncIndicator(id);
    else
      remote->RequestSyncAll();
  }
  else if (signal_name == "ActiveMenuPointerMotion")
  {
    int x = 0;
    int y = 0;
    g_variant_get (parameters, "(ii)", &x, &y);
    remote->on_menu_pointer_moved.emit(x, y);
  }
  else if (signal_name == "EntryShowNowChanged")
  {
    gchar    *id = NULL;
    gboolean  show_now;

    g_variant_get (parameters, "(sb)", &id, &show_now);
    remote->SetEntryShowNow(id, show_now);

    g_free (id);
  }
}

void on_proxy_name_owner_changed(GDBusProxy* proxy, GParamSpec* pspec,
                                 DBusIndicators* remote)
{
  char* name_owner = g_dbus_proxy_get_name_owner(proxy);

  if (name_owner == NULL)
  {
    // The panel service has stopped for some reason.  Restart it if not in
    // dev mode
    if (!g_getenv("UNITY_DEV_MODE"))
      remote->Reconnect();
  }

  g_free (name_owner);
}

void request_sync(GDBusProxy* proxy, const char* method, GVariant* name, SyncData* data)
{
  g_dbus_proxy_call(proxy, method, name, G_DBUS_CALL_FLAGS_NONE,
                    -1, data->cancel, on_sync_ready_cb, data);
}

void on_sync_ready_cb(GObject* source, GAsyncResult* res, gpointer data)
{
  SyncData* sync_data = reinterpret_cast<SyncData*>(data);
  GError* error = NULL;

  GVariant* args = g_dbus_proxy_call_finish((GDBusProxy*)source, res, &error);

  if (args == NULL)
  {
    g_warning("Unable to perform Sync() on panel service: %s", error->message);
    g_error_free(error);
    return;
  }

  sync_data->self->Sync(args, sync_data);
  g_variant_unref(args);
}

bool send_show_entry(ShowEntryData *data)
{
  g_return_val_if_fail (data != NULL, FALSE);
  g_return_val_if_fail (G_IS_DBUS_PROXY (data->proxy), FALSE);

  /* Re-flush 'cos X is crap like that */
  Display* d = nux::GetGraphicsDisplay()->GetX11Display();
  XFlush (d);

  g_dbus_proxy_call(data->proxy,
                     "ShowEntry",
                     g_variant_new("(suiii)",
                                   data->entry_id.c_str(),
                                    0,
                                    data->x,
                                    data->y,
                                    data->button),
                     G_DBUS_CALL_FLAGS_NONE,
                     -1,
                     NULL,
                     NULL,
                     NULL);
  delete data;
  return FALSE;
}

} // anonymous namespace

} // namespace indicator
} // namespace unity
