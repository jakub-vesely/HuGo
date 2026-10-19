"""Regression checks for BLE startup windows, without radio hardware."""
import ast
from pathlib import Path
import types
import unittest
from unittest.mock import Mock

SRC = Path(__file__).resolve().parents[1]


class BlePowerStartupTest(unittest.TestCase):
    def setUp(self):
        # Execute the real class, excluding imports requiring MicroPython.
        tree = ast.parse((SRC / "devel/___basal/___ble.py").read_text())
        node = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "Ble")
        self.power = Mock()
        self.planner = Mock()
        namespace = dict(PowerPlan=object, PowerMgmt=self.power,
                         Planner=self.planner, ActiveVariable=Mock(),
                         RemoteKey=Mock(), Logging=Mock(), BleLogger=Mock())
        exec(compile(ast.Module(body=[node], type_ignores=[]), "ble.py", "exec"), namespace)
        self.ble = namespace["Ble"]
        self.ble._start_ble = Mock()
        self.ble._stop_ble = Mock()
        self.power.get_plan.return_value = self.plan(0, 0, 0)
        self.ble.init()

    @staticmethod
    def plan(initial=20, running=1, down=60):
        return types.SimpleNamespace(ble_plan=types.SimpleNamespace(
            initial_time_up=initial, running_time_up=running, time_down=down))

    def test_boot_plan_keeps_ble_up_for_twenty_checks(self):
        self.assertEqual(self.ble._time_to_power_save, 0)
        self.ble._set_power_save_timeouts(self.plan())
        self.ble._check_time_to_power_save(True)
        self.assertEqual(self.ble._time_to_power_save, 20)
        for _ in range(19):
            self.ble._check_time_to_power_save(False)
        self.ble._stop_ble.assert_not_called()
        self.power.unblock_power_save.assert_not_called()
        self.ble._check_time_to_power_save(False)
        self.ble._stop_ble.assert_called_once_with()
        self.power.unblock_power_save.assert_called_once_with()
        self.planner.postpone.assert_called_with(60, self.ble._check_time_to_power_save, True)

    def test_plan_loaded_after_planner_started_gets_initial_window(self):
        self.ble._check_time_to_power_save(True)
        self.ble._set_power_save_timeouts(self.plan())
        self.assertEqual(self.ble._time_to_power_save, 20)

    def test_later_wakeup_uses_one_second_window(self):
        self.ble._set_power_save_timeouts(self.plan())
        self.ble.just_initialized = False
        self.ble._time_to_power_save = 0
        self.ble._check_time_to_power_save(True)
        self.assertEqual(self.ble._time_to_power_save, 1)
        self.ble._check_time_to_power_save(False)
        self.ble._stop_ble.assert_called_once_with()

    def test_connected_device_keeps_sleep_blocked_on_plan_change(self):
        self.ble._connections.add(1)
        self.ble._time_to_power_save = 0
        self.ble._set_power_save_timeouts(self.plan())
        self.ble._check_time_to_power_save(True)
        for _ in range(25):
            self.ble._check_time_to_power_save(False)
        self.ble._stop_ble.assert_not_called()
        self.power.unblock_power_save.assert_not_called()


if __name__ == "__main__":
    unittest.main()
