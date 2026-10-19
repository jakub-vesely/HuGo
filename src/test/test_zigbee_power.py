"""Power-management integration checks with simulated machine/radio APIs."""
from pathlib import Path
import sys
import types
import unittest
from unittest.mock import Mock, call, patch

SRC = Path(__file__).resolve().parents[1]


class ZigbeePowerTest(unittest.TestCase):
    def setUp(self):
        self.machine = Mock()
        self.radio = Mock()
        self.radio.READ = 1
        self.radio.REPORTING = 4
        self.power = types.ModuleType("___basal.___power_mgmt")
        self.wrapper = types.ModuleType("zigbee_wrapper_test")
        dependencies = {
            "machine": self.machine,
            "os": types.SimpleNamespace(uname=lambda: types.SimpleNamespace(
                machine="ESP32H2 module with ESP32-H2")),
            "zigbee": self.radio,
            "___basal": types.ModuleType("___basal"),
            "___basal.___power_mgmt": self.power,
        }
        with patch.dict(sys.modules, dependencies):
            exec((SRC / "devel/___basal/___power_mgmt.py").read_text(), self.power.__dict__)
            exec((SRC / "devel/___basal/___zigbee.py").read_text(), self.wrapper.__dict__)
        self.mgmt = self.power.PowerMgmt
        self.mgmt.set_plan(self.power.PowerPlan(
            True, 64000000, self.power.BlePowerPlan(15, 1, 60)))
        self.machine.reset_mock()

    def test_no_zigbee_keeps_manual_sleep(self):
        self.mgmt.light_sleep(1000)
        self.machine.lightsleep.assert_called_once_with(1000)
        self.radio.idle.assert_not_called()

    def test_each_role_uses_coordinated_idle(self):
        for role, args in (("start_end_device", ()), ("start_router", (8,))):
            with self.subTest(role=role):
                self.radio.reset_mock()
                getattr(self.wrapper, role)(*args)
                getattr(self.radio, role).assert_called_once_with(*args)
                self.mgmt.light_sleep(59000)
                self.radio.idle.assert_called_once_with(59000)
                self.machine.lightsleep.assert_not_called()

    def test_start_failure_does_not_install_idle_handler(self):
        self.radio.start_end_device.side_effect = OSError("start failed")
        with self.assertRaises(OSError):
            self.wrapper.start_end_device()
        self.assertIsNone(self.mgmt._light_sleep_handler)

    def test_idle_failure_restores_frequency_and_propagates(self):
        self.wrapper.start_end_device()
        self.radio.idle.side_effect = RuntimeError("PM configuration failed")
        with self.assertRaisesRegex(RuntimeError, "PM configuration failed"):
            self.mgmt.light_sleep(1000)
        self.assertEqual(self.machine.freq.call_args_list,
                         [call(32000000), call(64000000)])
        self.machine.lightsleep.assert_not_called()

    def test_plan_and_ble_block_are_both_respected(self):
        self.assertTrue(self.mgmt.is_power_save_enabled())
        self.mgmt.block_power_save()
        self.assertFalse(self.mgmt.is_power_save_enabled())
        self.mgmt.unblock_power_save()
        self.assertTrue(self.mgmt.is_power_save_enabled())
        self.mgmt.set_plan(self.power.PowerPlan.get_max_performance_plan())
        self.assertFalse(self.mgmt.is_power_save_enabled())


if __name__ == "__main__":
    unittest.main()
