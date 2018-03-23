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

#include <X11/extensions/shape.h>

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

using namespace unity;

namespace unity
{
WindowManagerPtr create_window_manager()
{
  return WindowManagerPtr(new XWindowManager());
}
}

struct StandaloneDndManager : XdndManager
{
  int Monitor() const { return 0; }
};

struct LauncherWindow
{
  LauncherWindow()
    : wt(nux::CreateGUIThread("Unity Launcher", 100, 100, 0, &LauncherWindow::ThreadWidgetInit, this))
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
    const auto width = launcher_controller->launcher().GetWidth();
    const auto height = workarea_geom.GetHeight();
    const auto x = workarea_geom.GetPosition().x;
    const auto y = workarea_geom.GetPosition().y;

    return nux::Rect(x, y, width, height);
  }

  nux::Rect WorkareaGeomLocal() const // window-space
  {
    return nux::Rect(0, 0, workarea_geom.GetWidth(), workarea_geom.GetHeight());
  }

  nux::Rect LauncherGeomLocal() const // window-space
  {
    const auto workarea_geom_local = WorkareaGeomLocal();
    const auto x_diff = workarea_geom_local.GetPosition().x - workarea_geom.GetPosition().x;
    const auto y_diff = workarea_geom_local.GetPosition().y - workarea_geom.GetPosition().y;
    return nux::Rect(launcher_geom.GetPosition().x + x_diff, launcher_geom.GetPosition().y + y_diff,
                     launcher_geom.GetWidth(), launcher_geom.GetHeight());
  }

  nux::Rect DashGeom() const // display-space
  {
    const auto width = workarea_geom.GetWidth() - launcher_geom.GetWidth();
    const auto height = workarea_geom.GetHeight();
    const auto x = launcher_geom.GetPosition().x + launcher_geom.GetWidth();
    const auto y = workarea_geom.GetPosition().y;

    return nux::Rect(x, y, width, height);
  }

  nux::Rect ScreenGeom() const // display-space
  {
    UScreen* uscreen = UScreen::GetDefault();
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
    auto &settings = Settings::Instance();
    settings.form_factor = unity::FormFactor::DESKTOP;
    settings.launcher_position = unity::LauncherPosition::LEFT;
    settings.is_standalone = true;
    settings.low_gfx = true;
  }

  void SetupBackground()
  {
    nux::ColorLayer color_layer(nux::color::Transparent);
    // nux::ColorLayer color_layer(nux::Color(0, 1, 0, 0.1));
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
    const std::vector<long> dock_values =
    {
      launcher_geom.GetWidth(), 0, // left, right
      0, 0, // top, bottom
      launcher_geom.GetPosition().y, launcher_geom.GetPosition().y + launcher_geom.GetHeight(), // left start y, left end y
      0, 0, // right start y, right end y
      0, 0, // top start x, top end x
      0, 0, // bottom start x, bottom end x
    };
    XChangeProperty(dpy, window_id, XInternAtom(dpy, "_NET_WM_STRUT_PARTIAL", False),
                    XA_CARDINAL, 32, PropModeReplace,
                    (unsigned char *)&dock_values[0], dock_values.size());

    XMoveWindow(dpy, window_id, launcher_geom.GetPosition().x, launcher_geom.GetPosition().y);
    XResizeWindow(dpy, window_id, workarea_geom.GetWidth(), launcher_geom.GetHeight());

    // setup input shape
    const auto &geom = LauncherGeomLocal(); // XShape seems to accept geoms in window-space
    XRectangle input_rectangle = {};
    input_rectangle.x = geom.GetPosition().x;
    input_rectangle.y = geom.GetPosition().y;
    input_rectangle.width = geom.GetWidth();
    input_rectangle.height = geom.GetHeight();
    XShapeCombineRectangles(dpy, window_id, ShapeInput, 0, 0, &input_rectangle, 1, ShapeSet, 0);

    XFlush(dpy);
  }

  void SetupLauncher()
  {
    launcher_controller->launcher().Resize(nux::Point(), launcher_geom.GetHeight());
  }

  void SetupDash()
  {

  }

  void SetupUBusInterests()
  {
    ubus_manager.RegisterInterest(UBUS_OVERLAY_SHOWN, [this] (GVariant *) {
      Display *dpy = nux::GetGraphicsDisplay()->GetX11Display();
      Window window_id = nux::GetGraphicsDisplay()->GetWindowHandle();

      // set input shape to whole workarea
      const auto geom = WorkareaGeomLocal();
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
      const auto geom = LauncherGeomLocal();
      XRectangle input_rectangle = {};
      input_rectangle.x = geom.GetPosition().x;
      input_rectangle.y = geom.GetPosition().y;
      input_rectangle.width = geom.GetWidth();
      input_rectangle.height = geom.GetHeight();
      XShapeCombineRectangles(dpy, window_id, ShapeInput, 0, 0, &input_rectangle, 1, ShapeSet, 0);

      XUngrabKeyboard(dpy, CurrentTime);
    });
  }

  void Init()
  {
    // FIXME: will attempt to cal XWindowManager
    // need to be called after window is created
    static unity::BGHash bghash;

    SetupSettings();

    launcher_controller.reset(new launcher::Controller(std::make_shared<StandaloneDndManager>(), std::make_shared<ui::EdgeBarrierController>()));
    dash_controller.reset(new dash::Controller());

    screen_geom = ScreenGeom();
    workarea_geom = WorkareaGeom();
    launcher_geom = LauncherGeom(); // FIXME: order matters :(
    dash_geom = DashGeom();

    SetupBackground();
    SetupLauncher();
    SetupDash();
    SetupUBusInterests();
    SetupWindow(); // FIXME: order matters :(
  }

  static void ThreadWidgetInit(nux::NThread* thread, void* self)
  {
    static_cast<LauncherWindow*>(self)->Init();
  }

  internal::FavoriteStoreGSettings favorite_store; // XXX: segfaults w/o this
  unity::Settings settings;
  unity::FontSettings font_settings;
  panel::Style panel_style;
  unity::ThumbnailGenerator thumb_generator;
  dash::Style dash_style;
  std::shared_ptr<nux::WindowThread> wt;
  nux::NuxTimerTickSource tick_source;
  nux::animation::AnimationController animation_controller;
  launcher::Controller::Ptr launcher_controller;
  input::Monitor im;
  nux::Rect launcher_geom;
  nux::Rect screen_geom;
  nux::Rect workarea_geom;
  dash::Controller::Ptr dash_controller;
  nux::Rect dash_geom;
  unity::UBusManager ubus_manager;
};

int main(int argc, char** argv)
{
  gtk_init(&argc, &argv);
  nux::NuxInitialize(nullptr);

  nux::logging::configure_logging(::getenv("UNITY_LOG_SEVERITY"));

  LauncherWindow lc;
  lc.Show();

  return 0;
}
