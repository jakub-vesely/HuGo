"""Host-side behavior checks; no radio or I2C hardware is exercised."""
import importlib.util
from pathlib import Path
import sys
import types
import unittest
from unittest.mock import Mock, patch

SRC = Path(__file__).resolve().parents[1]


class State:
    def __init__(self, value=False):
        self.value = value
        self.reports = []
        self.delivered = True

    def get(self):
        return self.value

    def set(self, value):
        self.value = value

    def report(self):
        self.reports.append(self.value)
        return self.delivered


class Led:
    def __init__(self):
        self.state = False

    def set_on(self):
        self.state = True

    def set_off(self):
        self.state = False


class ZigbeeIoTest(unittest.TestCase):
    def setUp(self):
        self.radio = Mock()
        self.radio.status.return_value = {"ready": True}
        self.radio.joined.return_value = True
        self.radio.add_button.return_value = State()
        self.radio.add_led.return_value = State()
        basal = types.ModuleType("basal")
        basal.zigbee = self.radio
        dependencies = {"basal": basal}
        for name in ("esp32", "machine", "neopixel", "basal.planner",
                     "basal.active_variable", "basal.logging",
                     "blocks.button_block", "blocks.rgb_led_block"):
            dependencies[name] = Mock()
        dependencies["blocks.rgb_led_block"].RgbLedBlock = Led
        dependencies["blocks.button_block"].ButtonBlock.return_value.value = Mock()
        dependencies["blocks.button_block"].ButtonBlock.return_value.value.get.return_value = False
        dependencies["neopixel"].NeoPixel.return_value = Mock()
        dependencies["neopixel"].NeoPixel.return_value.__setitem__ = Mock()
        spec = importlib.util.spec_from_file_location("zigbee_events_test", SRC / "examples/zigbee/events.py")
        module = importlib.util.module_from_spec(spec)
        with patch.dict(sys.modules, dependencies):
            spec.loader.exec_module(module)
            self.plan = module.Plan()

    def press(self, value):
        self.plan.button.value.get.return_value = value
        self.plan._button_state_changed()

    def test_press_and_release_report_button_and_drive_led(self):
        self.press(True)
        self.assertTrue(self.plan.button_led.state)
        self.assertTrue(self.plan.zigbee_led.get())
        self.assertEqual(self.plan.zigbee_button.reports, [True])
        self.press(False)
        self.assertFalse(self.plan.button_led.state)
        self.assertEqual(self.plan.zigbee_button.reports, [True, False])

    def test_remote_control_does_not_change_physical_button(self):
        self.plan.zigbee_led.set(True)
        self.plan._sync_zigbee_io()
        self.assertTrue(self.plan.button_led.state)
        self.assertFalse(self.plan.zigbee_button.get())
        self.plan.zigbee_led.set(False)
        self.plan._sync_zigbee_io()
        self.assertFalse(self.plan.button_led.state)

    def test_latest_local_or_remote_state_wins(self):
        self.press(True)
        self.plan.zigbee_led.set(False)
        self.plan._sync_zigbee_io()
        self.assertFalse(self.plan.button_led.state)
        self.press(False)
        self.plan.zigbee_led.set(True)
        self.press(True)
        self.press(False)
        self.plan._sync_zigbee_io()
        self.assertFalse(self.plan.zigbee_led.get())
        self.assertFalse(self.plan.button_led.state)

    def test_local_control_while_offline_and_report_after_join(self):
        self.radio.joined.return_value = False
        self.press(True)
        self.assertTrue(self.plan.button_led.state)
        self.assertEqual(self.plan.zigbee_button.reports, [])
        self.radio.joined.return_value = True
        self.plan._report_io_states()
        self.assertEqual(self.plan.zigbee_button.reports, [True])
        self.assertEqual(self.plan.zigbee_led.reports, [True])

    def test_local_change_before_stack_ready_is_applied_later(self):
        self.radio.status.return_value = {"ready": False}
        self.radio.joined.return_value = False
        self.press(True)
        self.assertTrue(self.plan.button_led.state)
        self.assertTrue(self.plan._pending_led)
        self.radio.status.return_value = {"ready": True}
        self.plan._sync_zigbee_io()
        self.assertTrue(self.plan.zigbee_led.get())
        self.assertFalse(self.plan._pending_led)

    def test_failed_report_is_retried(self):
        self.plan.zigbee_button.delivered = False
        self.press(True)
        self.assertTrue(self.plan._report_button)
        self.plan.zigbee_button.delivered = True
        self.plan._send_io_reports()
        self.assertFalse(self.plan._report_button)

    def test_reporting_does_not_overwrite_remote_state(self):
        self.press(True)
        self.plan.zigbee_led.set(False)
        self.plan._report_io_states()
        self.assertFalse(self.plan.button_led.state)
        self.assertEqual(self.plan.zigbee_led.reports[-1], False)


class ZigbeeDefinitionTest(unittest.TestCase):
    def test_standard_endpoint_and_attribute_definitions(self):
        native = types.ModuleType("zigbee")
        native.__getattr__ = lambda name: 0
        native.READ, native.REPORTING, native.BOOL = 1, 4, 0x10
        native.configure = Mock()
        native.add_endpoint = Mock(side_effect=lambda *a, **kw: Mock())
        spec = importlib.util.spec_from_file_location("zigbee_api_test", SRC / "devel/___basal/___zigbee.py")
        module = importlib.util.module_from_spec(spec)
        with patch.dict(sys.modules, {"zigbee": native}):
            spec.loader.exec_module(module)
        button = module.add_button()
        led = module.add_led()
        self.assertIsInstance(button, module.BinaryState)
        self.assertIsInstance(led, module.BinaryState)
        native.add_endpoint.assert_any_call(4, device_id=0x0100)
        button_ep = module._endpoints[3]
        button_ep.add_cluster.assert_called_once_with(0x000F)
        button_ep.add_cluster.return_value.add_attribute.assert_called_once_with(
            0x0055, 0x10, False, access=5)
        module._endpoints[4].add_cluster.assert_called_once_with(0x0006)
        with self.assertRaises(ValueError):
            module.add_led()


if __name__ == "__main__":
    unittest.main()
