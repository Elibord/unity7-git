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

#ifndef UNITYSHARED_XWINDOW_MANAGER_H
#define UNITYSHARED_XWINDOW_MANAGER_H

#include "unity-shared/WindowManager.h"

namespace unity
{

class XWindowManager : public WindowManager
{
public:
  XWindowManager();

  virtual Display* GetDisplay() const;

  Window GetActiveWindow() const override;
  std::vector<Window> GetWindowsInStackingOrder() const override;

  int MonitorGeometryIn(nux::Geometry const& geo) const override; // XXX: this seems to be used mostly in decor, unityshell and standalone window manager. also spread filter
  bool IsTopWindowFullscreenOnMonitorWithMouse() const override; // XXX: can't find where it's used

  bool IsWindowMaximized(Window window_id) const override;
  bool IsWindowVerticallyMaximized(Window window_id) const override;
  bool IsWindowHorizontallyMaximized(Window window_id) const override;
  bool IsWindowFullscreen(Window window_id) const override;
  bool IsWindowDecorated(Window window_id) const override;
  bool IsWindowOnCurrentDesktop(Window window_id) const override;
  bool IsWindowObscured(Window window_id) const override;
  bool IsWindowMapped(Window window_id) const override;
  bool IsWindowVisible(Window window_id) const override;
  bool IsWindowOnTop(Window window_id) const override;
  bool IsWindowShaded(Window window_id) const override;
  bool IsWindowClosable(Window window_id) const override;
  bool IsWindowMinimized(Window window_id) const override;
  bool IsWindowMinimizable(Window window_id) const override;
  bool IsWindowMaximizable(Window window_id) const override;
  bool HasWindowDecorations(Window window_id) const override;

  void ShowDesktop() override;
  bool InShowDesktop() const override;

  void ShowActionMenu(Time, Window, unsigned button, nux::Point const&) override;
  void Maximize(Window window_id) override;
  void VerticallyMaximize(Window window_id) override;
  void HorizontallyMaximize(Window window_id) override;
  void Restore(Window window_id) override;
  void RestoreAt(Window window_id, int x, int y) override;
  void Minimize(Window window_id) override;
  void UnMinimize(Window window_id) override;
  void Shade(Window window_id) override;
  void UnShade(Window window_id) override;
  void Close(Window window_id) override;

  void Activate(Window window_id) override;
  void Raise(Window window_id) override;
  void Lower(Window window_id) override;
  void RestackBelow(Window window_id, Window sibling_id) override; // XXX: doesn't seem to be used

  void TerminateScale() override;
  bool IsScaleActive() const override;
  bool IsScaleActiveForGroup() const override;

  void InitiateExpo() override;
  void TerminateExpo() override;
  bool IsExpoActive() const override;

  bool IsWallActive() const override; // used in SwitcherController (CanShowSwitcher)

  bool IsAnyWindowMoving() const override; // XXX: used in PanelView (panel)

  void FocusWindowGroup(std::vector<Window> const& windows,
                        FocusVisibility, int monitor = -1,
                        bool only_top_win = true) override;
  bool ScaleWindowGroup(std::vector<Window> const& windows,
                        int state, bool force) override;

  bool IsScreenGrabbed() const override; // used in DashController, HudController, LockScreenController, LockScreenPanel
  bool IsViewPortSwitchStarted() const override; // XXX: doesn't seem to be used

  void MoveResizeWindow(Window window_id, nux::Geometry geometry) override;
  void StartMove(Window window_id, int x, int y) override;
  void UnGrabMousePointer(Time, int button, int x, int y) override;

  int GetWindowMonitor(Window window_id) const override;
  nux::Geometry GetWindowGeometry(Window window_id) const override;
  nux::Geometry GetWindowSavedGeometry(Window window_id) const override;
  nux::Size GetWindowDecorationSize(Window window_id, Edge) const override;
  nux::Geometry GetScreenGeometry() const override;
  nux::Geometry GetWorkAreaGeometry(Window window_id = 0) const override;

  uint64_t GetWindowActiveNumber(Window window_id) const override;

  void SetWindowIconGeometry(Window window, nux::Geometry const& geo) override;

  void CheckWindowIntersections(nux::Geometry const& region, bool &active, bool &any) override;

  int WorkspaceCount() const override;

  nux::Point GetCurrentViewport() const override;
  void SetCurrentViewport(nux::Point const&) override;
  void SetViewportSize(int horizontal, int vertical) override;
  int GetViewportHSize() const override;
  int GetViewportVSize() const override;

  bool SaveInputFocus() override;
  bool RestoreInputFocus() override;

  std::string GetWindowName(Window window_id) const override;
  bool IsOnscreenKeyboard(Window window_id) const override;

private: // X-specific
  std::string GetStringProperty(Window window_id, Atom atom) const override;
  void SetCardinalProperty(Window, Atom, std::vector<long> const&) override;
  std::vector<long> GetCardinalProperty(Window, Atom) const override;

  Cursor GetCachedCursor(unsigned int cursor_name) const override;

protected:
  void AddProperties(debug::IntrospectionData& introspection) override;

private:
  Atom GetAtom(const std::string &name, bool only_if_exists=false) const;
  std::vector<Atom> GetWindowAtomsByAtom(Window window_id, Atom atom) const;
  int64_t GetWindowPropertyByAtom(Window window_id, Atom atom) const;
  void SetWindowState(Window window_id, long state_op, Atom atom1, Atom atom2) const;
};

}

#endif // WINDOW_MANAGER_H
