import time
import gc
from basal import zigbee
import esp32
from machine import Pin
from neopixel import NeoPixel
from basal.planner import Planner
from basal.active_variable import ActiveVariable
from basal.logging import Logging

class Plan():
    def __init__(self):
        self.logging = Logging("events")

        # Waveshare ESP32-H2 Mini (ESP32-H2-Zero): onboard WS2812 on GPIO8.
        self.led = NeoPixel(Pin(8, Pin.OUT), 1)
        # This board's LED uses RGB byte order instead of NeoPixel's default GRB.
        self.led.ORDER = (0, 1, 2, 3)
        self.blink_led((0, 32, 0))

        self.temperature = zigbee.add_sensor(zigbee.TEMPERATURE_MEASUREMENT)

        self.joined = ActiveVariable(False)
        self.joined.equal_to(True, self.blink_led, (32, 0, 0))
        self.joined.equal_to(True, self.send_temperature)
        self.joined.equal_to(False, self.join_zigbee)

        zigbee.start_end_device()
        Planner.postpone(1, self.join_zigbee)

    def blink_led(self, color):
        self.led[0] = color
        self.led.write()
        Planner.postpone(0.1, self.turn_off_led)

    def turn_off_led(self):
        self.led[0] = (0, 0, 0)
        self.led.write()

    def log_memory(self):
        idf_heaps = esp32.idf_heap_info(esp32.HEAP_DATA)
        python_free = gc.mem_free()
        python_allocated = gc.mem_alloc()
        idf_free = 0
        idf_largest_block = 0
        idf_minimum_free = 0
        for heap in idf_heaps:
            idf_free += heap[1]
            idf_largest_block = max(idf_largest_block, heap[2])
            idf_minimum_free += heap[3]
        self.logging.info(
            "RAM: Python free={} B, allocated={} B; "
            "IDF free={} B, largest block={} B, minimum free={} B".format(
                python_free,
                python_allocated,
                idf_free,
                idf_largest_block,
                idf_minimum_free,
            )
        )

    def send_temperature(self):
        self.logging.info(f"temperature: {esp32.mcu_temperature()} °C")
        if not zigbee.joined():
            self.joined.set(False)

        try:
            delivered = self.temperature.send(esp32.mcu_temperature())
            self.logging.info(f"temperature delivery confirmed: {delivered}")
            self.blink_led((0, 32, 0) if delivered else (0, 0, 32))
        except Exception as error:
            self.logging.error(f"try to send value via zigbee failed with an error: {error}")
            self.blink_led((0, 0, 32))
        self.log_memory()
        Planner.postpone(30, self.send_temperature)

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
