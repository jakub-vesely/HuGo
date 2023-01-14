#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from basal.logging import Logging
from blocks.meteo_block import MeteoBlock
from basal.remote_value import RemoteValue
from basal.active_variable import ActiveVariable
class Plan():
    def __init__(self) -> None:
        self.logging = Logging("Plan")

        self.meteo = MeteoBlock()

        RemoteValue.add("test.meteo.temperature", self.meteo.temperature)

        RemoteValue.add("test.meteo.pressure", self.meteo.pressure)
        RemoteValue.add("test.meteo.humidity", self.meteo.humidity)

        self.beat_status = False
        RemoteValue.add("test.main.status", ActiveVariable(initial_value=True, renew_period=1, renew_func=self.beats))

    def beats(self):
        self.logging.info("beat");
        self.beat_status = not self.beat_status
        return "1" if self.beat_status else "0"

Plan()
