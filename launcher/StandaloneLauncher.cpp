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

#include <Nux/Nux.h>
#include <Nux/NuxTimerTickSource.h>
#include <NuxCore/AnimationController.h>
#include <NuxCore/Logger.h>
#include <gtk/gtk.h>

#include "unity-shared/BackgroundEffectHelper.h"
#include "unity-shared/IconRenderer.h"
#include "unity-shared/InputMonitor.h"
#include "unity-shared/PanelStyle.h"
#include "unity-shared/UBusMessages.h"
#include "unity-shared/UnitySettings.h"
#include "unity-shared/UScreen.h"
#include "unity-shared/XWindowManager.h"
#include "EdgeBarrierController.h"
#include "FavoriteStoreGSettings.h"
#include "LauncherController.h"
#include "Launcher.h"
#include "QuicklistManager.h"

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
  nux::Rect LauncherGeom()
  {
    assert(controller);
    const long launcher_width = controller->launcher().GetWidth();

    // assuming there is no workarea reservation for launcher itself

    Display *dpy = nux::GetGraphicsDisplay()->GetX11Display();

    Atom ret = 0;
    int format = 0;
    unsigned char *data = 0;
    unsigned long nitems = 0, after = 0;

    int status = XGetWindowProperty(dpy, DefaultRootWindow(dpy),
                                    XInternAtom(dpy, "_NET_WORKAREA", False), 0, 4, False, XA_CARDINAL,
                                    &ret, &format, &nitems, &after, &data);


    long launcher_height = 0, launcher_x = 0, launcher_y = 0;
    if (status == Success && ret == XA_CARDINAL && format == 32 && nitems == 4)
    {
      long *workarea = reinterpret_cast<long *>(data);
      launcher_x = workarea[0];
      launcher_y = workarea[1];
      launcher_height = workarea[3];

      XFree(data);
    }

    // std::cout << "launcher geom: "
    //   << launcher_width << "x" << launcher_height
    //   << "+" << launcher_x << "-" << launcher_y
    //   << std::endl;

    return nux::Rect(launcher_x, launcher_y, launcher_width, launcher_height);
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
    XResizeWindow(dpy, window_id, launcher_geom.GetWidth(), launcher_geom.GetHeight());

    XFlush(dpy);
  }

  void SetupLauncher()
  {
    controller->launcher().Resize(nux::Point(), launcher_geom.GetHeight());
  }

  void Init()
  {
    controller.reset(new launcher::Controller(std::make_shared<StandaloneDndManager>(), std::make_shared<ui::EdgeBarrierController>()));
    launcher_geom = LauncherGeom();

    SetupBackground();
    SetupLauncher();
    SetupWindow();

    // wt->window_configuration.connect([this] (int x, int y, int w, int h) {
    //   nux::GetGraphicsDisplay()->ResetWindowSize();
    // });

    QuicklistManager::Default()->quicklist_opened.connect([this] (const nux::ObjectPtr<QuicklistView> &) {
      UScreen* uscreen = UScreen::GetDefault();
      const auto screen_width = uscreen->GetScreenGeometry().GetWidth();

      Display *dpy = nux::GetGraphicsDisplay()->GetX11Display();
      Window window_id = nux::GetGraphicsDisplay()->GetWindowHandle();

      XResizeWindow(dpy, window_id, screen_width, launcher_geom.GetHeight());

      XFlush(dpy);
    });

    QuicklistManager::Default()->quicklist_closed.connect([this] (const nux::ObjectPtr<QuicklistView> &) {
      Display *dpy = nux::GetGraphicsDisplay()->GetX11Display();
      Window window_id = nux::GetGraphicsDisplay()->GetWindowHandle();

      XResizeWindow(dpy, window_id, launcher_geom.GetWidth(), launcher_geom.GetHeight());

      XFlush(dpy);
    });
  }

  static void ThreadWidgetInit(nux::NThread* thread, void* self)
  {
    static_cast<LauncherWindow*>(self)->Init();
  }

  internal::FavoriteStoreGSettings favorite_store;
  unity::Settings settings;
  panel::Style panel_style;
  std::shared_ptr<nux::WindowThread> wt;
  nux::NuxTimerTickSource tick_source;
  nux::animation::AnimationController animation_controller;
  launcher::Controller::Ptr controller;
  input::Monitor im;
  nux::Rect launcher_geom;
};

int main(int argc, char** argv)
{
  gtk_init(&argc, &argv);
  nux::logging::configure_logging(::getenv("UNITY_LOG_SEVERITY"));
  nux::NuxInitialize(0);

  LauncherWindow lc;
  lc.Show();

  return 0;
}
