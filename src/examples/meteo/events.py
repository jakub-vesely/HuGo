#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from basal.logging import Logging
from blocks.power_block import PowerBlock
from blocks.meteo_block import MeteoBlock
from basal.remote_value import RemoteValue
from basal.active_variable import ActiveVariable
from basal.power_mgmt import PowerMgmt, PowerPlan, BlePowerPlan

class Plan():
    def __init__(self) -> None:
        self.logging = Logging("Plan")

        self.meteo = MeteoBlock(measurement_period=120)
        self.power = PowerBlock(measurement_period=120)

        RemoteValue.add("test.meteo.temperature", self.meteo.temperature)
        RemoteValue.add("test.meteo.pressure", self.meteo.pressure)
        RemoteValue.add("test.meteo.humidity", self.meteo.humidity)

        RemoteValue.add("test.power.is_usb_connected", self.power.is_usb_connected)
        RemoteValue.add("test.power.is_charging", self.power.is_charging)
        RemoteValue.add("test.power.battery_voltage", self.power.battery_voltage)
        RemoteValue.add("test.power.battery_current", self.power.battery_current)

        self.beat_status = False
        RemoteValue.add("test.main.status", ActiveVariable(initial_value=True, renew_period=120, renew_func=self.beats))

        PowerMgmt.set_plan(PowerPlan(True, PowerPlan.freq_min, BlePowerPlan(90, 1, 120)))

    def beats(self):
        self.logging.info("beat");
        self.beat_status = not self.beat_status
        return 1 if self.beat_status else 0

Plan()
