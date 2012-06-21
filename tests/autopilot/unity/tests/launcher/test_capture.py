# -*- Mode: Python; coding: utf-8; indent-tabs-mode: nil; tab-width: 4 -*-
# Copyright 2012 Canonical
# Authors: Thomi Richards,
#          Marco Trevisan (Treviño)
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 3, as published
# by the Free Software Foundation.

from __future__ import absolute_import

from autopilot.emulators.X11 import ScreenGeometry
from autopilot.matchers import Eventually
import logging
from testtools.matchers import Equals, LessThan, GreaterThan
from time import sleep

from unity.tests import UnityTestCase

logger = logging.getLogger(__name__)


class LauncherCaptureTests(UnityTestCase):
    """Test the launchers ability to capture/not capture the mouse."""

    screen_geo = ScreenGeometry()

    def setHideMode(self, mode):
        launcher = self.launcher.get_launcher_for_monitor(0)
        self.assertThat(launcher.hidemode, Eventually(Equals(mode)))

    def leftMostMonitor(self):
        x1, y1, width, height = self.screen_geo.get_monitor_geometry(0)
        x2, y2, width, height = self.screen_geo.get_monitor_geometry(1)

        if x1 < x2:
            return 0
        return 1

    def rightMostMonitor(self):
        return 1 - self.leftMostMonitor()

    def setUp(self):
        super(LauncherCaptureTests, self).setUp()
        self.set_unity_option('launcher_capture_mouse', True)
        self.set_unity_option('launcher_hide_mode', 0)
        self.set_unity_option('num_launchers', 0)
        self.setHideMode(0)

    def test_launcher_captures_while_sticky_and_revealed(self):
        """Tests that the launcher captures the mouse when moving between monitors
        while revealed.
        """
        if self.screen_geo.get_num_monitors() <= 1:
            self.skipTest("Cannot run this test with a single monitor configured.")

        x, y, width, height = self.screen_geo.get_monitor_geometry(self.rightMostMonitor())
        self.mouse.move(x + width / 2, y + height / 2, False)
        self.mouse.move(x - width / 2, y + height / 2, True, 5, .002)

        x_fin, y_fin = self.mouse.position()
        # The launcher should have held the mouse a little bit
        self.assertThat(x_fin, GreaterThan(x - width / 2))

    def test_launcher_not_capture_while_not_sticky_and_revealed(self):
        """Tests that the launcher doesn't captures the mouse when moving between monitors
        while revealed and stick is off.
        """
        if self.screen_geo.get_num_monitors() <= 1:
            self.skipTest("Cannot run this test with a single monitor configured.")

        self.set_unity_option('launcher_capture_mouse', False)

        x, y, width, height = self.screen_geo.get_monitor_geometry(self.rightMostMonitor())
        self.mouse.move(x + width / 2, y + height / 2, False)
        self.mouse.move(x - width / 2, y + height / 2, True, 5, .002)

        x_fin, y_fin = self.mouse.position()
        # The launcher should have held the mouse a little bit
        self.assertThat(x_fin, Equals(x - width / 2))

    def test_launcher_not_capture_while_not_sticky_and_hidden_moving_right(self):
        """Tests that the launcher doesn't capture the mouse when moving between monitors
        while hidden and sticky is off.
        """
        if self.screen_geo.get_num_monitors() <= 1:
            self.skipTest("Cannot run this test with a single monitor configured.")

        self.set_unity_option('launcher_hide_mode', 1)
        self.set_unity_option('launcher_capture_mouse', False)

        self.setHideMode(1)

        x, y, width, height = self.screen_geo.get_monitor_geometry(self.leftMostMonitor())
        self.mouse.move(x + width / 2, y + height / 2, False)
        sleep(1.5)
        self.mouse.move(x + width * 1.5, y + height / 2, True, 5, .002)

        x_fin, y_fin = self.mouse.position()
        # The launcher should have held the mouse a little bit
        self.assertThat(x_fin, Equals(x + width * 1.5))

    def test_launcher_capture_while_sticky_and_hidden_moving_right(self):
        """Tests that the launcher captures the mouse when moving between monitors
        while hidden.
        """
        if self.screen_geo.get_num_monitors() <= 1:
            self.skipTest("Cannot run this test with a single monitor configured.")

        self.set_unity_option('launcher_hide_mode', 1)

        self.setHideMode(1)

        x, y, width, height = self.screen_geo.get_monitor_geometry(self.leftMostMonitor())
        self.mouse.move(x + width / 2, y + height / 2, False)
        sleep(1.5)
        self.mouse.move(x + width * 1.5, y + height / 2, True, 5, .002)

        x_fin, y_fin = self.mouse.position()
        # The launcher should have held the mouse a little bit
        self.assertThat(x_fin, LessThan(x + width * 1.5))
