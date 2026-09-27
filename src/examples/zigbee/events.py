import time
from basal import zigbee
import esp32
from basal.planner import Planner
from basal.active_variable import ActiveVariable
from basal.logging import Logging


class Plan():
    def __init__(self):
        self.logging = Logging("events")

        self.temperature = zigbee.add_sensor(zigbee.TEMPERATURE_MEASUREMENT)

        self.joined = ActiveVariable(False)
        self.joined.equal_to(True, self.send_temperature)
        self.joined.equal_to(False, self.join_zigbee)

        zigbee.start()
        Planner.postpone(1, self.join_zigbee)

    def send_temperature(self):
        self.logging.info(f"temperature: {esp32.mcu_temperature()} °C")
        if not zigbee.joined():
            self.joined.set(False)

        try:
            self.temperature.send(esp32.mcu_temperature())
        except Exception as error:
            self.logging.error(f"try to send value via zigbee failed with an error: {error}")
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
