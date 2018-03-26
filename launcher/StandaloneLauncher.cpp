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

#include <thread>
#include <X11/extensions/shape.h>
#include <X11/XKBlib.h>

#include <Nux/Nux.h>
#include <Nux/NuxTimerTickSource.h>
#include <NuxCore/AnimationController.h>
#include <NuxCore/Logger.h>
#include <gtk/gtk.h>

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
#include "dash/ApplicationStarterImp.h"
#include "dash/DashController.h"
#include "EdgeBarrierController.h"
#include "FavoriteStoreGSettings.h"
#include "LauncherController.h"
#include "Launcher.h"

#include <UnityCore/GSettingsScopes.h>

namespace unity
{
WindowManagerPtr create_window_manager()
{
  return WindowManagerPtr(new XWindowManager());
}
}

struct StandaloneDndManager : unity::XdndManager
{
  int Monitor() const { return 0; }
};

struct LauncherWindow
{
  LauncherWindow()
    : wt(nux::CreateGUIThread("Unity Launcher", 0, 0, 0, &LauncherWindow::ThreadWidgetInit, this))
    , animation_controller(tick_source)
  {}

  void Show()
  {
    wt->Run(nullptr);
  }

private:
  nux::Rect LauncherGeom() const // display-space
  {
    assert(launcher_controller);

    if (unity::Settings::Instance().launcher_position == unity::LauncherPosition::LEFT)
    {
      const auto width = launcher_controller->launcher().GetWidth();
      const auto height = workarea_geom.GetHeight();
      const auto x = workarea_geom.GetPosition().x;
      const auto y = workarea_geom.GetPosition().y;

      return nux::Rect(x, y, width, height);
    }
    else  // BOTTOM
    {
      const auto width = workarea_geom.GetWidth();
      const auto height = launcher_controller->launcher().GetHeight();
      const auto x = workarea_geom.GetPosition().x;
      const auto y = workarea_geom.GetPosition().y + workarea_geom.GetHeight() - height;

      return nux::Rect(x, y, width, height);
    }
  }

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

    int status = XGetWindowProperty(dpy, DefaultRootWindow(dpy),
                                    XInternAtom(dpy, "_NET_WORKAREA", False), 0, 4, False, XA_CARDINAL,
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
    settings.form_factor = unity::FormFactor::DESKTOP;
    settings.launcher_position = unity::LauncherPosition::LEFT;
    settings.is_standalone = true;
    // settings.low_gfx = true;
  }

  void SetupPanel()
  {
    const auto panel_height = workarea_geom.GetPosition().y - screen_geom.GetPosition().y;

    auto &panel_style = unity::panel::Style::Instance();
    panel_style.SetPanelHeight(panel_height);
  }

  void SetupBackground()
  {
    nux::ColorLayer color_layer(nux::color::Transparent);
    wt->SetWindowBackgroundPaintLayer(&color_layer);
  }

  void SetupWindow()
  {
    Display *dpy = nux::GetGraphicsDisplay()->GetX11Display();
    Window window_id = nux::GetGraphicsDisplay()->GetWindowHandle();

    // set type to dock
    auto atom = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DOCK", False);
    XChangeProperty(dpy, window_id, XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False),
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
    XChangeProperty(dpy, window_id, XInternAtom(dpy, "_MOTIF_WM_HINTS", False),
                    XA_CARDINAL, 32, PropModeReplace,
                    (unsigned char *)&hints_values[0], hints_values.size());

    // reserve space
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
        launcher_geom.GetPosition().y, launcher_geom.GetPosition().y + launcher_geom.GetHeight(), // bottom start x, bottom end x
      });
    }
    XChangeProperty(dpy, window_id, XInternAtom(dpy, "_NET_WM_STRUT_PARTIAL", False),
                    XA_CARDINAL, 32, PropModeReplace,
                    (unsigned char *)&dock_values[0], dock_values.size());

    XMoveWindow(dpy, window_id, screen_geom.GetPosition().x, screen_geom.GetPosition().y);
    XResizeWindow(dpy, window_id, screen_geom.GetWidth(), screen_geom.GetHeight());

    // setup input shape
    const auto launcher_geom = LauncherGeom(); // XShape seems to accept geoms in window-space

    XRectangle input_rectangle = {};
    input_rectangle.x = launcher_geom.GetPosition().x;
    input_rectangle.y = launcher_geom.GetPosition().y;
    input_rectangle.width = launcher_geom.GetWidth();
    input_rectangle.height = launcher_geom.GetHeight();
    XShapeCombineRectangles(dpy, window_id, ShapeInput, 0, 0, &input_rectangle, 1, ShapeSet, 0);

    XFlush(dpy);
  }

  void SetupUBusInterests()
  {
    ubus_manager.RegisterInterest(UBUS_OVERLAY_SHOWN, [this] (GVariant *) {
      Display *dpy = nux::GetGraphicsDisplay()->GetX11Display();
      Window window_id = nux::GetGraphicsDisplay()->GetWindowHandle();

      // set input shape to whole workarea
      const auto geom = ScreenGeom();
      XRectangle input_rectangle = {};
      input_rectangle.x = geom.GetPosition().x;
      input_rectangle.y = geom.GetPosition().y;
      input_rectangle.width = geom.GetWidth();
      input_rectangle.height = geom.GetHeight();
      XShapeCombineRectangles(dpy, window_id, ShapeInput, 0, 0, &input_rectangle, 1, ShapeSet, 0);

      XGrabKeyboard(dpy, window_id, True, GrabModeAsync, GrabModeAsync, CurrentTime);
    });

    ubus_manager.RegisterInterest(UBUS_OVERLAY_HIDDEN, [this] (GVariant *) {
      Display *dpy = nux::GetGraphicsDisplay()->GetX11Display();
      Window window_id = nux::GetGraphicsDisplay()->GetWindowHandle();

      // set input to launcher
      const auto geom = LauncherGeom();
      XRectangle input_rectangle = {};
      input_rectangle.x = geom.GetPosition().x;
      input_rectangle.y = geom.GetPosition().y;
      input_rectangle.width = geom.GetWidth();
      input_rectangle.height = geom.GetHeight();
      XShapeCombineRectangles(dpy, window_id, ShapeInput, 0, 0, &input_rectangle, 1, ShapeSet, 0);

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
      { XKeysymToKeycode(dpy, XK_1), AnyModifier },
      { XKeysymToKeycode(dpy, XK_2), AnyModifier },
      { XKeysymToKeycode(dpy, XK_3), AnyModifier },
      { XKeysymToKeycode(dpy, XK_4), AnyModifier },
      { XKeysymToKeycode(dpy, XK_5), AnyModifier },
      { XKeysymToKeycode(dpy, XK_6), AnyModifier },
      { XKeysymToKeycode(dpy, XK_7), AnyModifier },
      { XKeysymToKeycode(dpy, XK_8), AnyModifier },
      { XKeysymToKeycode(dpy, XK_9), AnyModifier },
      { XKeysymToKeycode(dpy, XK_0), AnyModifier },
      { XKeysymToKeycode(dpy, XK_T), AnyModifier },
      { XKeysymToKeycode(dpy, XK_F1), AnyModifier },
      { XKeysymToKeycode(dpy, XK_Tab), AnyModifier },
      { XKeysymToKeycode(dpy, XK_Escape), Mod4Mask },
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
    // reference unityshell.cpp:UnityScreen::XModifiersToNux

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

    if (event.type != KeyPress && event.type != KeyRelease)
      return false;

    const int when = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch()).count();

    static const auto code_super_l = XKeysymToKeycode(dpy, XK_Super_L);
    static const auto code_super_r = XKeysymToKeycode(dpy, XK_Super_R);

    if (event.type == KeyPress)
    {
      if (event.xkey.keycode == code_super_l || event.xkey.keycode == code_super_r)
      {
        GrabSuperkeys();

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
        || keysym == XK_t || keysym == XK_T
        || keysym == XK_Escape)
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

  void Init()
  {
    // FIXME: will attempt to call XWindowManager
    // need to be called after window is created
    static unity::BGHash bghash;

    SetupSettings();
    SetupBackground();

    screen_geom = ScreenGeom();
    workarea_geom = WorkareaGeom();

    SetupPanel();

    launcher_controller.reset(new unity::launcher::Controller(
      std::make_shared<StandaloneDndManager>(),
      std::make_shared<unity::ui::EdgeBarrierController>()));
    dash_controller.reset(new unity::dash::Controller());

    launcher_geom = LauncherGeom(); // FIXME: order matters :(

    SetupUBusInterests();
    SetupWindow();
    SetupShortcuts();

    // WindowManager::Default().average_color = nux::Color(71.0f/255, 128.0f/255, 97.0f/255); // void
    // WindowManager::Default().average_color = nux::Color(0.0f/255, 128.0f/255, 0.0f/255); // emerald
    // WindowManager::Default().average_color = nux::Color(0.0f/255, 0.0f/255, 64.0f/255); // sapphire
    // WindowManager::Default().average_color = nux::Color(0.0f/255, 64.0f/255, 64.0f/255); // teal
    // WindowManager::Default().average_color = nux::Color(0.0f, 1.0f/255, 1.0f/255); // dark teal
    unity::WindowManager::Default().average_color = nux::Color(0.71f/255, 1.28f/255, 0.97f/255); // dark void
  }

  static void ThreadWidgetInit(nux::NThread* thread, void* self)
  {
    static_cast<LauncherWindow*>(self)->Init();
  }

  unity::internal::FavoriteStoreGSettings favorite_store; // XXX: segfaults w/o this
  unity::Settings settings;
  unity::FontSettings font_settings;
  unity::panel::Style panel_style;
  unity::ThumbnailGenerator thumb_generator;
  unity::dash::Style dash_style;
  std::shared_ptr<nux::WindowThread> wt;
  nux::NuxTimerTickSource tick_source;
  nux::animation::AnimationController animation_controller;
  unity::launcher::Controller::Ptr launcher_controller;
  unity::input::Monitor im;
  nux::Rect launcher_geom;
  nux::Rect screen_geom;
  nux::Rect workarea_geom;
  unity::dash::Controller::Ptr dash_controller;
  unity::UBusManager ubus_manager;
  std::vector<std::pair<KeyCode, unsigned> > superkeys;
};

int main(int argc, char **argv)
{
  gtk_init(&argc, &argv);
  nux::NuxInitialize(nullptr);

  nux::logging::configure_logging(::getenv("UNITY_LOG_SEVERITY"));

  LauncherWindow lc;
  lc.Show();

  return 0;
}
