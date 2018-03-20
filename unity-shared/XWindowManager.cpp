// -*- Mode: C++; indent-tabs-mode: nil; tab-width: 2 -*-
/*
 * Copyright (C) 2010-2012 Canonical Ltd
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

#include <Nux/Nux.h>
#include <NuxCore/Logger.h>
#include <gdk/gdkx.h>
#include "XWindowManager.h"

namespace unity
{
namespace
{
DECLARE_LOGGER(logger, "unity.windowmanager.x");

const long NET_WM_MOVERESIZE_MOVE = 8;

namespace atom
{
Atom _NET_WM_VISIBLE_NAME = 0;
Atom XA_COMPOUND_TEXT = 0;
Atom ONSCREEN_KEYBOARD = 0;
}
}

XWindowManager::XWindowManager()
{
  auto *dpy = GetDisplay();

  atom::_NET_WM_VISIBLE_NAME = XInternAtom(dpy, "_NET_WM_VISIBLE_NAME", False);
  atom::XA_COMPOUND_TEXT = XInternAtom(dpy, "COMPOUND_TEXT", False);
  atom::ONSCREEN_KEYBOARD = XInternAtom(dpy, "ONSCREEN_KEYBOARD", False);
}

Display* XWindowManager::GetDisplay() const
{
  return nux::GetGraphicsDisplay()->GetX11Display();
}

std::string XWindowManager::GetStringProperty(Window window_id, Atom atom) const
{
  auto *dpy = GetDisplay();

  Atom type;
  int result, format;
  unsigned long n_items, bytes_after;
  char *val = nullptr;

  result = XGetWindowProperty(dpy, window_id, atom, 0L, 65536, False,
                              AnyPropertyType, &type, &format, &n_items,
                              &bytes_after, reinterpret_cast<unsigned char **>(&val));

  if (result != Success)
  {
    LOG_DEBUG(logger) << "Impossible to get the property " << gdk_x11_get_xatom_name(atom)
                      << " for window " << window_id;
    return std::string();
  }

  if (!val || n_items == 0)
  {
    LOG_DEBUG(logger) << "Impossible to get the property " << gdk_x11_get_xatom_name(atom)
                      << " for window " << window_id << ": empty value";
    return std::string();
  }

  std::unique_ptr<char[], int(*)(void*)> string(val, XFree);

  if (format != 8)
  {
    LOG_ERROR(logger) << "Impossible to get the property " << gdk_x11_get_xatom_name(atom)
                      << " for window " << window_id << ": invalid format " << format;
    return std::string();
  }

  if (type != XA_STRING && type != atom::XA_COMPOUND_TEXT && type != XInternAtom(dpy, "UTF8_STRING", 0))
  {
    LOG_ERROR(logger) << "Impossible to get the property " << gdk_x11_get_xatom_name(atom)
                      << " for window " << window_id << ": invalid string type: "
                      << gdk_x11_get_xatom_name(type);
    return std::string();
  }

  if (type == atom::XA_COMPOUND_TEXT || (type == XA_STRING && !g_utf8_validate(val, n_items, nullptr)))
  {
    // In case we have compound text, we need to convert it to utf-8
    XTextProperty text_property;
    text_property.value = reinterpret_cast<unsigned char*>(val);
    text_property.encoding = type;
    text_property.format = format;
    text_property.nitems = n_items;

    char **list = nullptr;
    int count = 0;
    result = XmbTextPropertyToTextList(dpy, &text_property, &list, &count);

    if (result != Success || count == 0)
    {
      LOG_WARN(logger) << "Impossible to get the property " << gdk_x11_get_xatom_name(atom)
                       << "for window " << window_id << " properly: impossible to convert to current locale";
      return std::string(val, n_items);
    }

    std::unique_ptr<char*[], void(*)(char**)> list_ptr(list, XFreeStringList);

    if (count != 1)
    {
      LOG_WARN(logger) << "Impossible to get the property " << gdk_x11_get_xatom_name(atom)
                       << "for window " << window_id << " properly: invalid number of parsed values";
    }

    return list_ptr[0];
  }

  return std::string(val, n_items);
}

void XWindowManager::SetCardinalProperty(Window window_id, Atom atom, std::vector<long> const& values)
{
  auto *dpy = GetDisplay();

  XChangeProperty(dpy, window_id, atom, XA_CARDINAL, 32, PropModeReplace,
                  (unsigned char *) values.data(), values.size());
}

std::vector<long> XWindowManager::GetCardinalProperty(Window window_id, Atom atom) const
{
  auto *dpy = GetDisplay();

  Atom type;
  int result, format;
  unsigned long n_items, bytes_after;
  long *buf = nullptr;

  result = XGetWindowProperty(dpy, window_id, atom, 0L, 65536, False,
                              XA_CARDINAL, &type, &format, &n_items, &bytes_after,
                              reinterpret_cast<unsigned char **>(&buf));

  std::unique_ptr<long[], int(*)(void*)> buffer(buf, XFree);

  if (result == Success && type == XA_CARDINAL && format == 32 && buffer && n_items > 0)
  {
    std::vector<long> values(n_items);

    for (unsigned i = 0; i < n_items; ++i)
      values[i] = buffer[i];

    return values;
  }

  return std::vector<long>();
}

std::string XWindowManager::GetWindowName(Window window_id) const
{
  auto *dpy = GetDisplay();

  std::string name = GetStringProperty(window_id, atom::_NET_WM_VISIBLE_NAME);

  if (!name.empty())
    return name;

  name = GetStringProperty(window_id, XInternAtom(dpy, "_NET_WM_NAME", 0));

  if (!name.empty())
    return name;

  return GetStringProperty(window_id, XA_WM_NAME);
}

bool XWindowManager::IsOnscreenKeyboard(Window window_id) const
{
  std::vector<long> values = GetCardinalProperty(window_id, atom::ONSCREEN_KEYBOARD);

  if (values.empty())
    return false;

  return values[0] != 0;
}

Window XWindowManager::GetActiveWindow() const
{
  // TODO
  return 0;
}

std::vector<Window> XWindowManager::GetWindowsInStackingOrder() const
{
  std::vector<Window> ret;
  // TODO
  return ret;
}

int XWindowManager::MonitorGeometryIn(nux::Geometry const& geo) const
{
  // TODO
  return 0;
}

bool XWindowManager::IsTopWindowFullscreenOnMonitorWithMouse() const
{
  // TODO
  return false;
}

bool XWindowManager::IsWindowMaximized(Window window_id) const
{
  // TODO
  return false;
}

bool XWindowManager::IsWindowVerticallyMaximized(Window window_id) const
{
  // TODO
  return false;
}

bool XWindowManager::IsWindowHorizontallyMaximized(Window window_id) const
{
  // TODO
  return false;
}

bool XWindowManager::IsWindowFullscreen(Window window_id) const
{
  // TODO
  return false;
}

bool XWindowManager::IsWindowDecorated(Window window_id) const
{
  // TODO
  return false;
}

bool XWindowManager::IsWindowOnCurrentDesktop(Window window_id) const
{
  // TODO
  return true;
}

bool XWindowManager::IsWindowObscured(Window window_id) const
{
  // TODO
  return false;
}

bool XWindowManager::IsWindowMapped(Window window_id) const
{
  // TODO
  return true;
}

bool XWindowManager::IsWindowVisible(Window window_id) const
{
  // TODO
  return true;
}

bool XWindowManager::IsWindowOnTop(Window window_id) const
{
  // TODO
  return false;
}

bool XWindowManager::IsWindowShaded(Window window_id) const
{
  // TODO
  return false;
}

bool XWindowManager::IsWindowClosable(Window window_id) const
{
  // TODO
  return false;
}

bool XWindowManager::IsWindowMinimized(Window window_id) const
{
  // TODO
  return false;
}

bool XWindowManager::IsWindowMinimizable(Window window_id) const
{
  // TODO
  return false;
}

bool XWindowManager::IsWindowMaximizable(Window window_id) const
{
  // TODO
  return false;
}

bool XWindowManager::HasWindowDecorations(Window window_id) const
{
  // TODO
  return false;
}

void XWindowManager::ShowDesktop()
{
  // TODO
}

bool XWindowManager::InShowDesktop() const
{
  // TODO
  return false;
}

void XWindowManager::ShowActionMenu(Time, Window, unsigned button, nux::Point const&) 
{
  // TODO
}

void XWindowManager::Maximize(Window window_id)
{
  // TODO
}

void XWindowManager::VerticallyMaximize(Window window_id)
{
  // TODO
}

void XWindowManager::HorizontallyMaximize(Window window_id)
{
  // TODO
}

void XWindowManager::Restore(Window window_id)
{
  // TODO
}

void XWindowManager::RestoreAt(Window window_id, int x, int y)
{
  // TODO
}

void XWindowManager::Minimize(Window window_id)
{
  // TODO
}

void XWindowManager::UnMinimize(Window window_id)
{
  // TODO
}

void XWindowManager::Shade(Window window_id)
{
  // TODO
}

void XWindowManager::UnShade(Window window_id)
{
  // TODO
}

void XWindowManager::Close(Window window_id)
{
  // TODO
}

void XWindowManager::Activate(Window window_id)
{
  // TODO
}

void XWindowManager::Raise(Window window_id)
{
  // TODO
}

void XWindowManager::Lower(Window window_id)
{
  // TODO
}

void XWindowManager::RestackBelow(Window window_id, Window sibiling_id)
{
  // TODO
}

void XWindowManager::TerminateScale() 
{
  // TOOD
}

bool XWindowManager::IsScaleActive() const 
{
  // TOOD
  return false;
}

bool XWindowManager::IsScaleActiveForGroup() const 
{
  // TOOD
  return false;
}

void XWindowManager::InitiateExpo() 
{
  // TOOD
}

void XWindowManager::TerminateExpo() 
{
  // TOOD
}
bool XWindowManager::IsExpoActive() const 
{
  // TODO
  return false;
}

bool XWindowManager::IsWallActive() const 
{
  // TODO
  return false;
}

bool XWindowManager::IsAnyWindowMoving() const 
{
  // TOOD
  return false;
}

void XWindowManager::FocusWindowGroup(std::vector<Window> const& windows,
                                      FocusVisibility, int monitor/* = -1*/,
                                      bool only_top_win/* = true*/)
{
  // TODO
}

bool XWindowManager::ScaleWindowGroup(std::vector<Window> const& windows,
                                      int state, bool force)
{
  return false;
}

bool XWindowManager::IsScreenGrabbed() const 
{
  // TODO
  return false;
}

bool XWindowManager::IsViewPortSwitchStarted() const 
{
  // TODO
  return false;
}

void XWindowManager::MoveResizeWindow(Window window_id, nux::Geometry geometry)
{
  // TODO
}

void XWindowManager::UnGrabMousePointer(Time timestamp, int button, int x, int y)
{
  Display* dpy = GetDisplay();

  XUngrabPointer(dpy, CurrentTime);
  XFlush(dpy);

  // --------------------------------------------------------------------------
  // FIXME: This is a workaround until the non-paired events issue is fixed in
  // nux
  XButtonEvent bev;
  memset(&bev, 0, sizeof(bev));

  bev.type = ButtonRelease;
  bev.send_event = False;
  bev.display = dpy;
  bev.time = timestamp;
  bev.x = x;
  bev.y = y;
  bev.x_root = x;
  bev.y_root = y;
  bev.button = button;
  bev.same_screen = True;
  nux::GetWindowThread()->ProcessForeignEvent(reinterpret_cast<XEvent*>(&bev), nullptr);
  // --------------------------------------------------------------------------
}

void XWindowManager::StartMove(Window window_id, int x, int y)
{
  if (x < 0 || y < 0)
    return;

  XEvent ev;
  Display* d = nux::GetGraphicsDisplay()->GetX11Display();

  /* We first need to ungrab the pointer. FIXME: Evil */
  UnGrabMousePointer(CurrentTime, Button1, x, y);

  ev.xclient.type    = ClientMessage;
  ev.xclient.display = d;

  ev.xclient.serial     = 0;
  ev.xclient.send_event = true;

  ev.xclient.window     = window_id;
  ev.xclient.message_type = XInternAtom(d, "_NET_WM_MOVERESIZE", 0);
  ev.xclient.format     = 32;

  ev.xclient.data.l[0] = x; // x_root
  ev.xclient.data.l[1] = y; // y_root
  ev.xclient.data.l[2] = NET_WM_MOVERESIZE_MOVE; //direction
  ev.xclient.data.l[3] = 1; // button
  ev.xclient.data.l[4] = 2; // source

  XSendEvent(d, DefaultRootWindow(d), FALSE,
             SubstructureRedirectMask | SubstructureNotifyMask,
             &ev);

  XSync(d, FALSE);
}

int XWindowManager::GetWindowMonitor(Window window_id) const 
{
  // TODO
  return 0;
}

nux::Geometry XWindowManager::GetWindowGeometry(Window window_id) const 
{
  // TODO
  return nux::Geometry(0, 0, 1, 1);
}

nux::Geometry XWindowManager::GetWindowSavedGeometry(Window window_id) const 
{
  // TODO
  return nux::Geometry();
}

nux::Size XWindowManager::GetWindowDecorationSize(Window window_id, Edge) const 
{
  // TODO
  return nux::Size();
}

nux::Geometry XWindowManager::GetScreenGeometry() const 
{
  // TODO
  return nux::Geometry(0, 0, 1024, 768);
}

nux::Geometry XWindowManager::GetWorkAreaGeometry(Window window_id/* = 0*/) const 
{
  // TODO
  return nux::Geometry(0, 0, 1024, 768);
}

uint64_t XWindowManager::GetWindowActiveNumber(Window window_id) const
{
  // TODO
  return 0;
}

void XWindowManager::SetWindowIconGeometry(Window window, nux::Geometry const& geo)
{
  // TODO
}

void XWindowManager::CheckWindowIntersections(nux::Geometry const& region, bool &active, bool &any) 
{
  // TODO
}

int XWindowManager::WorkspaceCount() const 
{
  // TODO
  return 1;
}

nux::Point XWindowManager::GetCurrentViewport() const
{
  // TODO
  return nux::Point();
}

void XWindowManager::SetCurrentViewport(nux::Point const&) 
{
  // TODO
}

void XWindowManager::SetViewportSize(int horizontal, int vertical) 
{
  // TODO
}

int XWindowManager::GetViewportHSize() const
{
  return 1024;
}

int XWindowManager::GetViewportVSize() const 
{
  // TODO
  return 1024;
}

bool XWindowManager::SaveInputFocus()
{
  // TODO
  return false;
}

bool XWindowManager::RestoreInputFocus()
{
  // TODO
  return false;
}

Cursor XWindowManager::GetCachedCursor(unsigned int cursor_name) const 
{
  return 0;
}

void XWindowManager::AddProperties(debug::IntrospectionData& wrapper) 
{
  // XXX: probably could be moved to base WindowSystem
  wrapper.add(GetScreenGeometry())
         .add("workspace_count", WorkspaceCount())
         .add("active_window", GetActiveWindow())
         .add("screen_grabbed", IsScreenGrabbed())
         .add("scale_active", IsScaleActive())
         .add("scale_active_for_group", IsScaleActiveForGroup())
         .add("expo_active", IsExpoActive())
         .add("viewport_switch_running", IsViewPortSwitchStarted())
         .add("showdesktop_active", InShowDesktop());
}

} // unity namespace
