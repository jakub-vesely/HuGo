from basal import zigbee
import esp32
from machine import Pin
from neopixel import NeoPixel
from basal.planner import Planner
from basal.active_variable import ActiveVariable
from basal.logging import Logging
from basal.power_mgmt import PowerMgmt, PowerPlan, BlePowerPlan

ENABLE_BUTTONS = False
TEMPERATURE_INTERVAL = 30

if ENABLE_BUTTONS:
    from blocks.button_block import ButtonBlock
    from blocks.rgb_led_block import RgbLedBlock

class Plan():
    def __init__(self):
        self.logging = Logging("events")

        # Waveshare ESP32-H2 Mini (ESP32-H2-Zero): onboard WS2812 on GPIO8.
        self.led = NeoPixel(Pin(8, Pin.OUT), 1)
        # This board's LED uses RGB byte order instead of NeoPixel's default GRB.
        self.led.ORDER = (0, 1, 2, 3)
        self.blink_led((0, 32, 0))

        if ENABLE_BUTTONS:
            self.button = ButtonBlock(measurement_period=0.1)
            self.button_led = RgbLedBlock()
            self.button_led.set_off()
            self.zigbee_button = zigbee.add_button()
            self.zigbee_led = zigbee.add_led()
            self._pending_button = False
            self._pending_led = False
            self._report_button = True
            self._report_led = True
            self.button.value.changed(self._button_state_changed)

        self.temperature = zigbee.add_sensor(zigbee.TEMPERATURE_MEASUREMENT)

        self.joined = ActiveVariable(False)
        self.joined.equal_to(True, self.blink_led, (32, 0, 0))
        self.joined.equal_to(True, self.send_temperature)
        if ENABLE_BUTTONS:
            self.joined.equal_to(True, self._report_io_states)
        self.joined.equal_to(False, self.join_zigbee)

        zigbee.start_end_device()
        Planner.postpone(1, self.join_zigbee)
        if ENABLE_BUTTONS:
            Planner.repeat(0.1, self._sync_zigbee_io)
            Planner.repeat(5, self._send_io_reports)

        PowerMgmt.set_plan(PowerPlan(True, PowerPlan.freq_min, BlePowerPlan(20, 1, 60)))

    def _button_state_changed(self):
        pressed = bool(self.button.value.get())
        self._set_led(pressed)
        self.logging.info("button pressed" if pressed else "button released")
        self._pending_button = True
        self._pending_led = True
        self._report_button = True
        self._report_led = True
        # Commit local changes immediately so the next remote command can win.
        self._sync_zigbee_io()
        self._send_io_reports()

    def _set_led(self, enabled):
        if enabled:
            self.button_led.set_on()
        else:
            self.button_led.set_off()

    def _sync_zigbee_io(self):
        if not zigbee.status()["ready"]:
            return
        try:
            if self._pending_button:
                self.zigbee_button.set(self.button.value.get())
                self._pending_button = False
            if self._pending_led:
                self.zigbee_led.set(bool(self.button_led.state))
                self._pending_led = False
            enabled = self.zigbee_led.get()
            if enabled != bool(self.button_led.state):
                self._set_led(enabled)
                self._report_led = True
                self.logging.info("remote LED on" if enabled else "remote LED off")
        except OSError as error:
            self.logging.error("Zigbee I/O sync failed:", error)

    def _report_io_states(self):
        self._report_button = True
        self._report_led = True
        self._send_io_reports()

    def _send_io_reports(self):
        if not zigbee.joined():
            return
        self._sync_zigbee_io()
        try:
            if self._report_button and not self._pending_button:
                self._report_button = not self.zigbee_button.report()
            if self._report_led and not self._pending_led:
                self._report_led = not self.zigbee_led.report()
        except OSError as error:
            self.logging.error("Zigbee I/O report failed:", error)

    def blink_led(self, color):
        self.led[0] = color
        self.led.write()
        Planner.postpone(0.1, self.turn_off_led)

    def turn_off_led(self):
        self.led[0] = (0, 0, 0)
        self.led.write()

    def send_temperature(self):
        if not zigbee.joined():
            self.joined.set(False)
            return

        try:
            # The current native API still waits internally for confirmation.
            delivered = self.temperature.send(esp32.mcu_temperature())
            self.blink_led((0, 32, 0) if delivered else (0, 0, 32))
        except Exception as error:
            self.logging.error(f"try to send value via zigbee failed with an error: {error}")
            self.blink_led((0, 0, 32))
        # Planner uses PowerMgmt to sleep between scheduled tasks, including BLE.
        Planner.postpone(TEMPERATURE_INTERVAL, self.send_temperature)

    def join_zigbee(self):
        status = zigbee.status()
        self.logging.info(f"joining zigbee status: {status}")
        if  zigbee.joined():
            self.joined.set(True)
            return

        if status["rejoining"]:
            self.logging.info("rejoining the saved Zigbee network")
        elif status["steering_status"] == 3 and not status["steering_in_progress"]:
            self.logging.error(
                "Zigbee network not found; make sure ZHA 'Add device' is still open"
            )
        try:
            zigbee.join()
        except OSError as e:
            self.logging.error("join error:", e)
        Planner.postpone(10, self.join_zigbee)

Plan()
