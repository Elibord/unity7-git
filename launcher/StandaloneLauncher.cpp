/*
 * Copyright 2010 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the  Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 3 along with this program.  If not, see
 * <http://www.gnu.org/licenses/>
 *
 * Authored by: Jason Smith <jason.smith@canonical.com>
 *
 */

#include <getopt.h>
#include <gtk/gtk.h>
#include <X11/extensions/shape.h>
#include <X11/XKBlib.h>
#include <thread>

#include <Nux/Nux.h>
#include <Nux/NuxTimerTickSource.h>
#include <NuxCore/AnimationController.h>
#include <NuxCore/Logger.h>

#include "unity-shared/BGHash.h"
#include "unity-shared/DashStyle.h"
#include "unity-shared/FontSettings.h"
#include "unity-shared/InputMonitor.h"
#include "unity-shared/PanelStyle.h"
#include "unity-shared/ThumbnailGenerator.h"
#include "unity-shared/UBusMessages.h"
#include "unity-shared/UnitySettings.h"
#include "unity-shared/UScreen.h"
#include "unity-shared/XWindowManager.h"
// #include "dash/ApplicationStarterImp.h"
#include "dash/DashController.h"
#include "EdgeBarrierController.h"
#include "FavoriteStoreGSettings.h"
#include "LauncherController.h"
#include "Launcher.h"

namespace
{
static const std::string window_title = "Chromatic";
static const std::string wm_class = "chromatic";

namespace atom
{
Atom _XROOTPMAP_ID = 0;
Atom _NET_WORKAREA = 0;
Atom _NET_WM_STRUT_PARTIAL = 0;
Atom _NET_WM_WINDOW_TYPE = 0;
Atom _NET_WM_WINDOW_TYPE_DOCK = 0;
Atom _MOTIF_WM_HINTS = 0;
} // namespace atom

struct options_t
{
  int lowgfx = 0;
  int bottom = 0;
  int netbook = 0;
} options;

} // namespace

namespace unity
{
WindowManagerPtr create_window_manager()
{
  return WindowManagerPtr(new XWindowManager());
}
} // namespace unity

namespace
{
nux::Color ComputeAverageWallpaperColor(Display *dpy)
{
  nux::Color average_color;

  auto root = DefaultRootWindow(dpy);

  XWindowAttributes attrs = {};
  XGetWindowAttributes(dpy, root, &attrs);

  Pixmap root_pixmap = None;
  Atom act_type = 0;
  int act_format = 0;
  unsigned long nitems = 0, bytes_after = 0;
  unsigned char *data = nullptr;

  if (XGetWindowProperty(dpy, root, atom::_XROOTPMAP_ID, 0, 1, False,
                         XA_PIXMAP, &act_type, &act_format, &nitems, &bytes_after, &data) == Success)
  {
    if (data != nullptr)
    {
      root_pixmap = *((Pixmap *) data);
      XFree(data);
    }
  }

  if (root_pixmap == None)
    return average_color;

  XImage *img = XGetImage(dpy, root_pixmap, 0, 0, attrs.width, attrs.height, ~0, ZPixmap);
  // XFreePixmap(dpy, root_pixmap);

  if (img == nullptr || img->depth < 24)
    return average_color;

  auto add_avg = [] (float prev_avg, float value, unsigned long samples)
  {
    return (prev_avg * samples + value) / (samples + 1);
  };

  float red = 0, green = 0, blue = 0;
  unsigned long samples = 0;
  for (int y = 0; y < img->height; ++y)
  {
    for (int x = 0; x < img->width; ++x)
    {
      const unsigned long pixel = XGetPixel(img,x,y);
      const float pixel_red = (pixel >> 16) / 255.0;
      const float pixel_green = ((pixel & 0xFF00) >> 8) / 255.0;
      const float pixel_blue = (pixel & 0xFF) / 255.0;

      red = add_avg(red, pixel_red, samples);
      green = add_avg(green, pixel_green, samples);
      blue = add_avg(blue, pixel_blue, samples);

      ++samples;
    }
  }

  XDestroyImage(img);

  average_color = nux::Color(red, green, blue, 1.0f);
  return average_color;
}
} // namespace

struct StandaloneDndManager : unity::XdndManager
{
  int Monitor() const { return 0; }
};

struct LauncherWindow
{
  LauncherWindow()
  : animation_controller(tick_source)
  {}

  static void ThreadWidgetInit(nux::NThread *thread, void *)
  {
    // LauncherWindow is need to be created after actual window is created
    // otherwise BGHash will crash
    static LauncherWindow self;
    self.Init();
  }

private:
  nux::Rect ScreenGeom() const // display-space
  {
    auto *uscreen = unity::UScreen::GetDefault();
    return uscreen->GetScreenGeometry();
  }

  nux::Rect WorkareaGeom() const // display-space
  {
    Display *dpy = nux::GetGraphicsDisplay()->GetX11Display();

    Atom ret = 0;
    int format = 0;
    unsigned char *data = 0;
    unsigned long nitems = 0, after = 0;

    int status = XGetWindowProperty(dpy, DefaultRootWindow(dpy), atom::_NET_WORKAREA,
                                    0, 4, False, XA_CARDINAL,
                                    &ret, &format, &nitems, &after, &data);

    long width = 0, height = 0, x = 0, y = 0;
    if (status == Success && ret == XA_CARDINAL && format == 32 && nitems == 4)
    {
      long *workarea = reinterpret_cast<long *>(data);
      x = workarea[0];
      y = workarea[1];
      width = workarea[2];
      height = workarea[3];

      XFree(data);
    }

    return nux::Rect(x, y, width, height);
  }

  void SetupSettings()
  {
    auto &settings = unity::Settings::Instance();
    settings.is_standalone = true;
    settings.form_factor = (options.netbook ? unity::FormFactor::NETBOOK : unity::FormFactor::DESKTOP);
    settings.launcher_position = (options.bottom ? unity::LauncherPosition::BOTTOM : unity::LauncherPosition::LEFT);
    settings.low_gfx = options.lowgfx;
  }

  void SetupPanel()
  {
    const auto workarea_geom = WorkareaGeom();
    const auto screen_geom = ScreenGeom();

    // XXX: well, this will only work if there is one panel across all monitors
    // or panels oof the same height on each monitor
    // so this is only a workaround sadly
    const auto panel_height = workarea_geom.GetPosition().y - screen_geom.GetPosition().y;

    auto &panel_style = unity::panel::Style::Instance();
    for (size_t i = 0; i < unity::UScreen::GetDefault()->GetMonitors().size(); ++i)
      panel_style.SetPanelHeight(panel_height, i);
  }

  void SetupBackground()
  {
    nux::ColorLayer color_layer(nux::color::Transparent);
    nux::GetWindowThread()->SetWindowBackgroundPaintLayer(&color_layer);

    Display *dpy = nux::GetGraphicsDisplay()->GetX11Display();
    const auto color = ComputeAverageWallpaperColor(dpy);

    const double MAX_USHORT = std::numeric_limits<unsigned short>::max ();
    unsigned short compiz_color[4] = { 0 };
    compiz_color[0] = color.red * MAX_USHORT;
    compiz_color[1] = color.green * MAX_USHORT;
    compiz_color[2] = color.blue * MAX_USHORT;
    compiz_color[3] = MAX_USHORT; // alpha, currently unused

    // State::Running for smooth transition between colors
    // bghash will set WindowManager::average_color
    bghash.UpdateColor(compiz_color, nux::animation::Animation::State::Running);
  }

  void SetupWindow()
  {
    Display *dpy = nux::GetGraphicsDisplay()->GetX11Display();
    Window window_id = nux::GetGraphicsDisplay()->GetWindowHandle();

    // set type to dock
    auto atom = atom::_NET_WM_WINDOW_TYPE_DOCK;
    XChangeProperty(dpy, window_id, atom::_NET_WM_WINDOW_TYPE,
                    XA_ATOM, 32, PropModeReplace,
                    (unsigned char *)&atom, 1);

    // undecorate (for xfce)
    const std::vector<long> hints_values =
    {
      0x2, // flags
      0x0, // functions
      0x0, // decorations
      0x0, // inputMode
      0x0, // status
    };
    XChangeProperty(dpy, window_id, atom::_MOTIF_WM_HINTS,
                    XA_CARDINAL, 32, PropModeReplace,
                    (unsigned char *)&hints_values[0], hints_values.size());

    // set class hint (WM_CLASS)
    XClassHint class_hint;
    class_hint.res_name = (char *)wm_class.c_str();
    class_hint.res_class = (char *)window_title.c_str();
    XSetClassHint(dpy, window_id, &class_hint);

    XFlush(dpy);
  }

  void SetupUBusInterests()
  {
    ubus_manager.RegisterInterest(UBUS_OVERLAY_SHOWN, [this] (GVariant *) {
      Display *dpy = nux::GetGraphicsDisplay()->GetX11Display();
      Window window_id = nux::GetGraphicsDisplay()->GetWindowHandle();

      // set input shape to whole screen
      const auto geom = ScreenGeom();
      XRectangle input_rectangle = {};
      input_rectangle.x = geom.GetPosition().x;
      input_rectangle.y = geom.GetPosition().y;
      input_rectangle.width = geom.GetWidth();
      input_rectangle.height = geom.GetHeight();
      XShapeCombineRectangles(dpy, window_id, ShapeInput, 0, 0,
        &input_rectangle, 1, ShapeSet, 0);

      XGrabKeyboard(dpy, window_id, True, GrabModeAsync, GrabModeAsync, CurrentTime);
    });

    ubus_manager.RegisterInterest(UBUS_OVERLAY_HIDDEN, [this] (GVariant *) {
      SetupScreens(); // FIXME: it's overkill to SetupScreens() here

      Display *dpy = nux::GetGraphicsDisplay()->GetX11Display();
      XUngrabKeyboard(dpy, CurrentTime);
    });
  }

  void GrabSuper()
  {
    auto *dpy = nux::GetGraphicsDisplay()->GetX11Display();

    const KeyCode grabs[] =
    {
      XKeysymToKeycode(dpy, XK_Super_L),
      XKeysymToKeycode(dpy, XK_Super_R),
    };

    // grab keys
    const auto modifiers = AnyModifier;
    const auto grab_window = DefaultRootWindow(dpy);
    const auto owner_events  = True;
    const auto pointer_mode = GrabModeAsync;
    const auto keyboard_mode = GrabModeAsync;

    for (const auto &keycode : grabs)
    {
      XGrabKey(dpy, keycode, modifiers, grab_window, owner_events,
               pointer_mode, keyboard_mode);
    }

    XSync(dpy, False);
  }

  void GrabSuperkeys()
  {
    auto *dpy = nux::GetGraphicsDisplay()->GetX11Display();

    // grab keys
    const auto grab_window = DefaultRootWindow(dpy);
    const auto owner_events  = True;
    const auto pointer_mode = GrabModeAsync;
    const auto keyboard_mode = GrabModeAsync;

    for (const auto &superkey : superkeys)
    {
      const auto &keycode = superkey.first;
      const auto &mods = superkey.second;
      XGrabKey(dpy, keycode, mods, grab_window, owner_events,
               pointer_mode, keyboard_mode);
    }

    XSync(dpy, False);
  }

  void UngrabSuperkeys()
  {
    auto *dpy = nux::GetGraphicsDisplay()->GetX11Display();

    // grab keys
    const auto grab_window = DefaultRootWindow(dpy);

    for (const auto &superkey : superkeys)
    {
      const auto &keycode = superkey.first;
      const auto &mods = superkey.second;
      XUngrabKey(dpy, keycode, mods, grab_window);
    }

    XSync(dpy, False);
  }

  void SetupShortcuts()
  {
    auto *dpy = nux::GetGraphicsDisplay()->GetX11Display();

    // populate superkeys
    const std::vector<std::pair<KeyCode, unsigned>> codes =
    {
      { XKeysymToKeycode(dpy, XK_1), Mod4Mask }, // launchers: super+1-0
      { XKeysymToKeycode(dpy, XK_2), Mod4Mask },
      { XKeysymToKeycode(dpy, XK_3), Mod4Mask },
      { XKeysymToKeycode(dpy, XK_4), Mod4Mask },
      { XKeysymToKeycode(dpy, XK_5), Mod4Mask },
      { XKeysymToKeycode(dpy, XK_6), Mod4Mask },
      { XKeysymToKeycode(dpy, XK_7), Mod4Mask },
      { XKeysymToKeycode(dpy, XK_8), Mod4Mask },
      { XKeysymToKeycode(dpy, XK_9), Mod4Mask },
      { XKeysymToKeycode(dpy, XK_0), Mod4Mask },
      { XKeysymToKeycode(dpy, XK_T), Mod4Mask }, // trash: super+t
      { XKeysymToKeycode(dpy, XK_F1), Mod4Mask }, // keynav: super+f1
      { XKeysymToKeycode(dpy, XK_Tab), Mod4Mask }, // tab switch: super+tab
      { XKeysymToKeycode(dpy, XK_A), Mod4Mask }, // dash: applications.scope
      { XKeysymToKeycode(dpy, XK_F), Mod4Mask }, // dash: files.scope
      { XKeysymToKeycode(dpy, XK_V), Mod4Mask }, // dash: video.scope
      { XKeysymToKeycode(dpy, XK_M), Mod4Mask }, // dash: music.scope
      { XKeysymToKeycode(dpy, XK_C), Mod4Mask }, // dash: photo.scope
      { XKeysymToKeycode(dpy, XK_Escape), AnyModifier }, // cancel tab switch: super+esc, cancel keynav: esc
    };
    superkeys.assign(codes.begin(), codes.end());

    GrabSuper();

    // get events from nux, XSelectInput and XNextEvent in thread won't work
    nux::GetGraphicsDisplay()->AddEventFilter({[] (XEvent event, void *data) {
      auto *dis = static_cast<LauncherWindow *>(data);
      return dis->HandleEvent(event);
    }, this});
  }

  unsigned XModifiersToNux(unsigned input) const
  {
    // reference: unityshell.cpp:UnityScreen::XModifiersToNux

    unsigned modifiers = 0;

    if (input & Mod1Mask)
      modifiers |= nux::KEY_MODIFIER_ALT;

    if (input & ShiftMask)
      modifiers |= nux::KEY_MODIFIER_SHIFT;

    if (input & ControlMask)
      modifiers |= nux::KEY_MODIFIER_CTRL;

    if (input & Mod4Mask)
      modifiers |= nux::KEY_MODIFIER_SUPER;

    return modifiers;
  }

  bool HandleEvent(const XEvent &event)
  {
    // reference: unityshell.cpp:UnityScreen::handleEvent()

    Display *dpy = nux::GetGraphicsDisplay()->GetX11Display();

    if (event.type != KeyPress && event.type != KeyRelease
    && event.type != MotionNotify
    && event.type != PropertyNotify)
      return false;

    const int when = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch()).count();

    static const auto code_super_l = XKeysymToKeycode(dpy, XK_Super_L);
    static const auto code_super_r = XKeysymToKeycode(dpy, XK_Super_R);

    if (event.type == PropertyNotify)
    {
      if (event.xproperty.atom == atom::_XROOTPMAP_ID)
        SetupBackground();
    }
    else if (event.type == MotionNotify)
    {
      const auto x = event.xmotion.x_root, y = event.xmotion.y_root;
      const auto &launchers = launcher_controller->launchers();

      for (auto &launcher : launchers)
      {
        const auto geom = launcher->GetParent()->GetGeometry();

        if (geom.IsInside(nux::Point(x, y)))
        {
          if (launcher->Hidden())
            launcher->SetHidden(false);
        }
      }

      return false;
    }
    else if (event.type == KeyPress)
    {
      if (event.xkey.keycode == code_super_l || event.xkey.keycode == code_super_r)
      {
        GrabSuperkeys();

        if (launcher_controller->KeyNavIsActive())
          launcher_controller->KeyNavTerminate(false);

        launcher_controller->HandleLauncherKeyPress(when);
        return true;
      }
      // it's ok not to check if super is pressed
      // because numkeys won't be grabbed otherwise
      // and dock don't get keyboard focus too
      else if (!dash_controller->IsVisible())
      {
        const KeySym keysym = XkbKeycodeToKeysym(event.xany.display, event.xkey.keycode, 0, 0);

        // this is only launcher keys + trash
        if ((keysym >= XK_0 && keysym <= XK_9)
        || keysym == XK_t || keysym == XK_T)
        {
          launcher_controller->HandleLauncherKeyEvent(XModifiersToNux(event.xkey.state), keysym, event.xkey.time);
          return true;
        }
        else if (keysym == XK_F1)
        {
          launcher_controller->KeyNavGrab();
          return true;
        }
        else if (keysym == XK_Tab)
        {
          if (launcher_controller->KeyNavIsActive())
          {
            launcher_controller->KeyNavNext();
          }
          else
          {
            launcher_controller->KeyNavActivate();
          }
          return true;
        }
        else if (keysym == XK_A || keysym == XK_a
        || keysym == XK_F || keysym == XK_f
        || keysym == XK_V || keysym == XK_v
        || keysym == XK_M || keysym == XK_m
        || keysym == XK_C || keysym == XK_c)
        {
          const std::map<KeySym, std::string> shortcuts =
          {
            { XK_A, "applications.scope" },
            { XK_a, "applications.scope" },
            { XK_F, "files.scope" },
            { XK_f, "files.scope" },
            { XK_V, "video.scope" },
            { XK_v, "video.scope" },
            { XK_M, "music.scope" },
            { XK_m, "music.scope" },
            { XK_C, "photos.scope" },
            { XK_c, "photos.scope" },
          };

          const auto it = shortcuts.find(keysym);
          if (it != shortcuts.end())
          {
            const auto &scope = it->second;
            ubus_manager.SendMessage(UBUS_PLACE_ENTRY_ACTIVATE_REQUEST,
                                     g_variant_new("(sus)", scope.c_str(), unity::dash::GOTO_DASH_URI, ""));
            return true;
          }
        }
        else if (keysym == XK_Escape)
        {
          if (launcher_controller->KeyNavIsActive())
            launcher_controller->KeyNavTerminate(false);

          return true;
        }
      }
    } // KeyPress
    else if (event.type == KeyRelease)
    {
      const KeySym keysym = XkbKeycodeToKeysym(event.xany.display, event.xkey.keycode, 0, 0);

      if (event.xkey.keycode == code_super_l || event.xkey.keycode == code_super_r)
      {
        UngrabSuperkeys();

        const bool was_tap = launcher_controller->AboutToShowDash(true, when);
        launcher_controller->HandleLauncherKeyRelease(was_tap, when);

        if (was_tap)
        {
          if (!dash_controller->IsVisible())
          {
            launcher_controller->KeyNavTerminate(false); // ungrab (if any)
            if (dash_controller->ShowDash())
              ubus_manager.SendMessage(UBUS_PLACE_ENTRY_ACTIVATE_REQUEST,
                                       g_variant_new("(sus)", "home.scope", unity::dash::GOTO_DASH_URI, ""));
          }
          else
          {
            dash_controller->HideDash();
          }

          return true;
        }
        else if (launcher_controller->KeyNavIsActive())
        {
          // here both keynav and tab switch need to be handled
          // they both activate with super pressed (not like in Unity,
          // in Unity keynav is activated with alt)

          const bool grabbed = launcher_controller->IsLauncherGrabbed(); // launcher is grabbed when keynav is activated
          if (!grabbed)
            launcher_controller->KeyNavTerminate(!grabbed);

          return true;
        }
      }
      else if (keysym == XK_Escape)
      {
        if (launcher_controller->KeyNavIsActive())
        {
          launcher_controller->KeyNavTerminate(false);
          launcher_controller->HandleLauncherKeyRelease(false, when); // required to reset launcher highlighting
          return true;
        }
      }

    } // KeyRelease

    return false;
  }

  void SetupScreens()
  {
    // first get rid of old slots if any
    const auto &launchers = launcher_controller->launchers();
    for (auto &slot : launcher_slots)
      slot.disconnect();

    // connect all launchers so they trigger SetupScreen() on change
    // this shouldn't happen often: on resolution change and display on/off i think
    launcher_slots.clear();
    launcher_slots.push_back(
      launcher_controller->launcher().changed.connect([this] (const nux::Geometry &geom) {
        SetupScreens();
    }));

    const auto screen_geom = ScreenGeom();

    auto *dpy = nux::GetGraphicsDisplay()->GetX11Display();
    Window window_id = nux::GetGraphicsDisplay()->GetWindowHandle();

    // resize window to whole screen
    XMoveWindow(dpy, window_id, screen_geom.GetPosition().x, screen_geom.GetPosition().y);
    XResizeWindow(dpy, window_id, screen_geom.GetWidth(), screen_geom.GetHeight());

    // unreserve space
    const std::vector<long> unreserve_values =
    {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
    XChangeProperty(dpy, window_id, atom::_NET_WM_STRUT_PARTIAL,
                    XA_CARDINAL, 32, PropModeReplace,
                    (unsigned char *)&unreserve_values[0], unreserve_values.size());

    // FIXME: can't set strut in the middle of the screen with _NET_WM_STRUT_PARTIAL
    // this is a workaround: set strut to left-most and/or bottom-most launcher only
    size_t left_most = (size_t)(-1);
    size_t bottom_most = (size_t)(-1);
    for (size_t i = 0; i < launchers.size(); ++i)
    {
      const auto &launcher = launchers[i];
      const auto geom = launcher->GetParent()->GetGeometry();
      if (left_most == (size_t)(-1)
      || geom.GetPosition().x < launchers[left_most]->GetParent()->GetGeometry().GetPosition().x)
      {
        left_most = i;
      }

      if (bottom_most == (size_t)(-1)
      || geom.GetPosition().y > launchers[bottom_most]->GetParent()->GetGeometry().GetPosition().y)
      {
        bottom_most = i;
      }
    }

    // reserve space for launchers
    for (size_t i = 0; i < launchers.size(); ++i)
    {
      const auto &launcher = launchers[i];

      // switch launchers that not left- or bottom-most to autohide
      if (i != left_most && i != bottom_most)
      {
        launcher->OverrideHideMode(unity::launcher::LauncherHideMode::LAUNCHER_HIDE_AUTOHIDE);
        continue;
      }

      const auto launcher_geom = launcher->GetParent()->GetGeometry();

      std::vector<long> dock_values;
      if (unity::Settings::Instance().launcher_position == unity::LauncherPosition::LEFT)
      {
        dock_values.assign({
          launcher_geom.GetWidth(), 0, // left, right
          0, 0, // top, bottom
          launcher_geom.GetPosition().y, launcher_geom.GetPosition().y + launcher_geom.GetHeight(), // left start y, left end y
          0, 0, // right start y, right end y
          0, 0, // top start x, top end x
          0, 0, // bottom start x, bottom end x
        });
      }
      else // BOTTOM
      {
        dock_values.assign({
          0, 0, // left, right
          0, launcher_geom.GetHeight(), // top, bottom
          0, 0, // left start y, left end y
          0, 0, // right start y, right end y
          0, 0, // top start x, top end x
          launcher_geom.GetPosition().x, launcher_geom.GetPosition().x + launcher_geom.GetWidth(), // bottom start x, bottom end x
        });
      }
      XChangeProperty(dpy, window_id, atom::_NET_WM_STRUT_PARTIAL,
                      XA_CARDINAL, 32, (i == 0 ? PropModeReplace : PropModeAppend),
                      (unsigned char *)&dock_values[0], dock_values.size());
    }

    // restore input to whole screen
    XRectangle screen_rectangle = {};
    screen_rectangle.x = screen_geom.GetPosition().x;
    screen_rectangle.y = screen_geom.GetPosition().y;
    screen_rectangle.width = screen_geom.GetWidth();
    screen_rectangle.height = screen_geom.GetHeight();
    XShapeCombineRectangles(dpy, window_id, ShapeInput, 0, 0,
      &screen_rectangle, 1, ShapeSet, 0);

    // reconfigure input on launchers
    std::vector<XRectangle> input_rectangles(launchers.size());
    for (const auto &launcher : launchers)
    {
      const auto &launcher_geom = launcher->GetParent()->GetGeometry();

      XRectangle input_rectangle = {};
      input_rectangle.x = launcher_geom.GetPosition().x;
      input_rectangle.y = launcher_geom.GetPosition().y;
      input_rectangle.width = launcher_geom.GetWidth();
      input_rectangle.height = launcher_geom.GetHeight();

      input_rectangles.push_back(input_rectangle);
    }

    XShapeCombineRectangles(dpy, window_id, ShapeInput, 0, 0,
      &input_rectangles[0], input_rectangles.size(), ShapeSet, 0);

    XFlush(dpy);
  }

  void Init()
  {
    // init atoms
    Display *dpy = nux::GetGraphicsDisplay()->GetX11Display();
    atom::_XROOTPMAP_ID = XInternAtom(dpy, "_XROOTPMAP_ID", False);
    atom::_NET_WORKAREA = XInternAtom(dpy, "_NET_WORKAREA", False);
    atom::_NET_WM_STRUT_PARTIAL = XInternAtom(dpy, "_NET_WM_STRUT_PARTIAL", False);
    atom::_NET_WM_WINDOW_TYPE = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
    atom::_NET_WM_WINDOW_TYPE_DOCK = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DOCK", False);
    atom::_MOTIF_WM_HINTS = XInternAtom(dpy, "_MOTIF_WM_HINTS", False);

    // settings need to be setup first because it will trigger some signals and slot in other parts
    SetupSettings();
    SetupBackground();

    SetupPanel();

    launcher_controller.reset(new unity::launcher::Controller(
      std::make_shared<StandaloneDndManager>(),
      std::make_shared<unity::ui::EdgeBarrierController>()));
    dash_controller.reset(new unity::dash::Controller());

    SetupUBusInterests();
    SetupWindow();
    SetupScreens();
    SetupShortcuts();

    unity::WindowManager::Default().average_color = nux::Color(0.71f/255, 1.28f/255, 0.97f/255); // dark void
  }

  unity::internal::FavoriteStoreGSettings favorite_store; // XXX: segfaults w/o this
  unity::Settings settings;
  unity::FontSettings font_settings;
  unity::panel::Style panel_style;
  unity::ThumbnailGenerator thumb_generator;
  unity::dash::Style dash_style;
  nux::NuxTimerTickSource tick_source;
  nux::animation::AnimationController animation_controller;
  unity::launcher::Controller::Ptr launcher_controller;
  unity::input::Monitor im;
  unity::dash::Controller::Ptr dash_controller;
  unity::UBusManager ubus_manager;
  unity::BGHash bghash; // will attempt to call XWindowManager, need to be called after window is created
  //
  std::vector<std::pair<KeyCode, unsigned> > superkeys;
  std::vector<sigc::connection> launcher_slots;
};

void print_help()
{
  std::cout
    << "  --help, -h      this help screen" << std::endl
    << "  --lowgfx        enable low gfx mode" << std::endl
    << "  --bottom        position launcher at bottom" << std::endl
    << "  --netbook       netbook form-factor (dash)" << std::endl
    ;
}

int main(int argc, char **argv)
{
  int c = 0;
  while (true)
  {
    static struct option long_options[] =
    {
      { "lowgfx",  no_argument, &options.lowgfx,  1 },
      { "bottom",  no_argument, &options.bottom,  1 },
      { "netbook", no_argument, &options.netbook, 1 },
      { "help",    no_argument, 0, 'h'},
      { 0, 0, 0, 0 },
    };

    int option_index = 0;
    c = getopt_long(argc, argv, "h", long_options, &option_index);

    if (c < 0)
      break;

    switch (c)
    {
    case 0:
      if (long_options[option_index].flag != 0)
        break;
    case 'h':
      print_help();
      exit(1);
    default:
      abort();
    }
  }

  gtk_init(nullptr, nullptr);
  nux::NuxInitialize(nullptr);

  nux::logging::configure_logging(::getenv("UNITY_LOG_SEVERITY"));

  std::shared_ptr<nux::WindowThread> wt(
    nux::CreateGUIThread(window_title.c_str(), 0, 0, 0,
    &LauncherWindow::ThreadWidgetInit));
  wt->Run(nullptr);

  return 0;
}
