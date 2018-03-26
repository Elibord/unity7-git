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
const long _NET_WM_STATE_REMOVE = 0;
const long _NET_WM_STATE_ADD = 1;
// const long _NET_WM_STATE_TOGGLE = 2;

namespace atom
{
Atom _NET_WM_VISIBLE_NAME = 0;
Atom _NET_WM_STATE = 0;
Atom XA_COMPOUND_TEXT = 0;
Atom ONSCREEN_KEYBOARD = 0;
}
}

XWindowManager::XWindowManager()
{
  atom::_NET_WM_VISIBLE_NAME = GetAtom("_NET_WM_VISIBLE_NAME");
  atom::_NET_WM_STATE = GetAtom("_NET_WM_STATE");
  atom::XA_COMPOUND_TEXT = GetAtom("COMPOUND_TEXT");
  atom::ONSCREEN_KEYBOARD = GetAtom("ONSCREEN_KEYBOARD");
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

  if (type != XA_STRING && type != atom::XA_COMPOUND_TEXT && type != GetAtom("UTF8_STRING", false))
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
  std::string name = GetStringProperty(window_id, atom::_NET_WM_VISIBLE_NAME);

  if (!name.empty())
    return name;

  name = GetStringProperty(window_id, GetAtom("_NET_WM_NAME"));

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
  auto *dpy = GetDisplay();
  Window root_window = DefaultRootWindow(dpy);

  const auto active_window = GetWindowPropertyByAtom(root_window, GetAtom("_NET_ACTIVE_WINDOW"));
  if (active_window < 0)
    return 0;

  return active_window;
}

std::vector<Window> XWindowManager::GetWindowsInStackingOrder() const
{
  std::vector<Window> ret;
  // TODO
  return ret;
}

int XWindowManager::MonitorGeometryIn(nux::Geometry const& geo) const
{
  return 0;
}

bool XWindowManager::IsTopWindowFullscreenOnMonitorWithMouse() const
{
  return false;
}

bool XWindowManager::IsWindowMaximized(Window window_id) const
{
  const auto atoms = GetWindowAtomsByAtom(window_id, atom::_NET_WM_STATE);
  const auto it_vert = std::find(atoms.begin(), atoms.end(), GetAtom("_NET_WM_STATE_MAXIMIZED_VERT"));
  const auto it_horz = std::find(atoms.begin(), atoms.end(), GetAtom("_NET_WM_STATE_MAXIMIZED_HORZ"));

  return (it_vert != atoms.end() && it_horz != atoms.end());
}

bool XWindowManager::IsWindowVerticallyMaximized(Window window_id) const
{
  const auto atoms = GetWindowAtomsByAtom(window_id, atom::_NET_WM_STATE);
  const auto it = std::find(atoms.begin(), atoms.end(), GetAtom("_NET_WM_STATE_MAXIMIZED_VERT"));

  return (it != atoms.end());
}

bool XWindowManager::IsWindowHorizontallyMaximized(Window window_id) const
{
  const auto atoms = GetWindowAtomsByAtom(window_id, atom::_NET_WM_STATE);
  const auto it = std::find(atoms.begin(), atoms.end(), GetAtom("_NET_WM_STATE_MAXIMIZED_HORZ"));

  return (it != atoms.end());
}

bool XWindowManager::IsWindowFullscreen(Window window_id) const
{
  const auto atoms = GetWindowAtomsByAtom(window_id, atom::_NET_WM_STATE);
  const auto it = std::find(atoms.begin(), atoms.end(), GetAtom("_NET_WM_STATE_FULLSCREEN"));

  return (it != atoms.end());
}

bool XWindowManager::IsWindowDecorated(Window window_id) const
{
  // TODO
  return true;
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
  return true;
}

bool XWindowManager::IsWindowMinimized(Window window_id) const
{
  const auto atoms = GetWindowAtomsByAtom(window_id, atom::_NET_WM_STATE);
  const auto it = std::find(atoms.begin(), atoms.end(), GetAtom("_NET_WM_STATE_HIDDEN"));

  return (it != atoms.end());
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
  return true;
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
  // https://standards.freedesktop.org/wm-spec/wm-spec-1.3.html#idm140130317598336
  SetWindowState(window_id, _NET_WM_STATE_ADD, GetAtom("_NET_WM_STATE_MAXIMIZED_VERT"), GetAtom("_NET_WM_STATE_MAXIMIZED_HORZ"));
}

void XWindowManager::VerticallyMaximize(Window window_id)
{
  SetWindowState(window_id, _NET_WM_STATE_ADD, GetAtom("_NET_WM_STATE_MAXIMIZED_VERT"), 0);
}

void XWindowManager::HorizontallyMaximize(Window window_id)
{
  SetWindowState(window_id, _NET_WM_STATE_ADD, GetAtom("_NET_WM_STATE_MAXIMIZED_HORZ"), 0);
}

void XWindowManager::Restore(Window window_id)
{
  SetWindowState(window_id, _NET_WM_STATE_REMOVE, GetAtom("_NET_WM_STATE_MAXIMIZED_VERT"), GetAtom("_NET_WM_STATE_MAXIMIZED_HORZ"));
}

void XWindowManager::RestoreAt(Window window_id, int x, int y)
{
  Restore(window_id);

  auto *dpy = GetDisplay();
  XMoveWindow(dpy, window_id, x, y);
}

void XWindowManager::Minimize(Window window_id)
{
  SetWindowState(window_id, _NET_WM_STATE_ADD, GetAtom("_NET_WM_STATE_HIDDEN"), 0);
}

void XWindowManager::UnMinimize(Window window_id)
{
  SetWindowState(window_id, _NET_WM_STATE_REMOVE, GetAtom("_NET_WM_STATE_HIDDEN"), 0);
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
  auto *dpy = GetDisplay();

  XEvent ev;
  ev.type                 = ClientMessage;
  ev.xclient.window       = window_id;
  ev.xclient.message_type = GetAtom("WM_PROTOCOLS"); // XXX: probably suboptimal to call XInternAtom() every time
  ev.xclient.format       = 32;
  ev.xclient.data.l[0]    = GetAtom("WM_DELETE_WINDOW");
  ev.xclient.data.l[1]    = CurrentTime;
  ev.xclient.data.l[2]    = 0;
  ev.xclient.data.l[3]    = 0;
  ev.xclient.data.l[4]    = 0;

  XSendEvent(dpy, window_id, False, NoEventMask, &ev);
}

void XWindowManager::Activate(Window window_id)
{
  auto *dpy = GetDisplay();

  XEvent ev;
  ev.xclient.type = ClientMessage;
  ev.xclient.serial = 0;
  ev.xclient.send_event = True;
  ev.xclient.display = dpy;
  ev.xclient.window = window_id;
  ev.xclient.message_type = GetAtom("_NET_ACTIVE_WINDOW");
  ev.xclient.format = 32;
  ev.xclient.data.l[0] = 1; // CLIENT_TYPE_APPLICATION // https://specifications.freedesktop.org/wm-spec/1.3/ar01s03.html
  ev.xclient.data.l[1] = CurrentTime;
  ev.xclient.data.l[2] = 0;
  ev.xclient.data.l[3] = 0;
  ev.xclient.data.l[4] = 0;

  XSendEvent(dpy, DefaultRootWindow(dpy), False, SubstructureRedirectMask | SubstructureNotifyMask, &ev);
}

void XWindowManager::Raise(Window window_id)
{
  auto *dpy = GetDisplay();
  XRaiseWindow(dpy, window_id);
}

void XWindowManager::Lower(Window window_id)
{
  auto *dpy = GetDisplay();
  XLowerWindow(dpy, window_id);
}

void XWindowManager::RestackBelow(Window window_id, Window sibling_id)
{
  // https://tronche.com/gui/x/xlib/window/XRestackWindows.html
  auto *dpy = GetDisplay();
  Window windows[2] = { sibling_id, window_id };
  XRestackWindows(dpy, windows, 2);
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
  // TODO
  return false;
}

void XWindowManager::FocusWindowGroup(std::vector<Window> const& windows,
                                      FocusVisibility, int monitor/* = -1*/,
                                      bool only_top_win/* = true*/)
{
  if (windows.empty())
    return;

  const Window active_window = GetActiveWindow();
  const auto it = std::find(windows.begin(), windows.end(), active_window);
  const bool any_active = (it != windows.end());

  // if no window is active - active top most one
  if (!any_active)
  {
    Activate(windows[0]);
  }
  // if already active - activate next window or cycle through
  else if (windows.size() > 1)
  {
    Activate(it == windows.end() - 1 ? windows[0] : *(it + 1));
  }
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
  ev.xclient.message_type = GetAtom("_NET_WM_MOVERESIZE");
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

Atom XWindowManager::GetAtom(const std::string &name, bool only_if_exists) const
{
  auto *dpy = GetDisplay();
  return XInternAtom(dpy, name.c_str(), only_if_exists);
}

std::vector<Atom> XWindowManager::GetWindowAtomsByAtom(Window window_id, Atom atom) const
{
  std::vector<Atom> atoms; // ret

  auto *dpy = GetDisplay();

  Atom type = None;
  int format = 0;
  unsigned long nitems = 0;
  unsigned long bytes_after = 0;
  Atom *data = nullptr;
  int result = Success;

  result = XGetWindowProperty(dpy, window_id, atom,
                              0, 65535,
                              False, XA_ATOM, &type, &format, &nitems,
                              &bytes_after, (unsigned char**)&data);

  if (result != Success)
    return atoms;

  if (type != XA_ATOM)
  {
    XFree (data);
    return atoms;
  }

  atoms.resize(nitems);
  atoms.assign(data, data + nitems);

  XFree (data);

  return atoms;
}

int64_t XWindowManager::GetWindowPropertyByAtom(Window window_id, Atom atom) const
{
  auto *dpy = GetDisplay();

  Atom actual_type = 0;
  int actual_format = 0;
  unsigned long nitems = 0;
  /*unsigned long nbytes;*/
  unsigned long bytes_after = 0; /* unused */
  unsigned char *prop = nullptr;

  int status = XGetWindowProperty(dpy, window_id, atom, 0, (~0L),
                                  False, AnyPropertyType, &actual_type,
                                  &actual_format, &nitems, &bytes_after,
                                  &prop);

  if (status != Success)
    return -1;

  long ret = 0;
  switch (actual_format)
  {
  case 32:
    ret = *reinterpret_cast<int32_t *>(prop);
    break;
  case 16:
    ret = *reinterpret_cast<int16_t *>(prop);
    break;
  case 8:
    ret = *reinterpret_cast<int8_t *>(prop);
    break;
  default:
    ret = -1;
  }

  XFree(prop);

  return ret;
}

void XWindowManager::SetWindowState(Window window_id, long state_op, Atom atom1, Atom atom2) const
{
  auto *dpy = GetDisplay();
  Window root_window = DefaultRootWindow(dpy);

  XEvent xev;
  xev.xclient.type = ClientMessage;
  xev.xclient.serial = 0;
  xev.xclient.send_event = True;
  xev.xclient.display = dpy;
  xev.xclient.window = window_id;
  xev.xclient.message_type = atom::_NET_WM_STATE;
  xev.xclient.format = 32;
  xev.xclient.data.l[0] = state_op;
  xev.xclient.data.l[1] = atom1;
  xev.xclient.data.l[2] = atom2;
  xev.xclient.data.l[3] = 1; // CLIENT_TYPE_APPLICATION;
  xev.xclient.data.l[4] = 0;

  XSendEvent(dpy, root_window, False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
}

} // unity namespace
