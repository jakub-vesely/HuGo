#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from basal.logging import Logging
from blocks.meteo_block import MeteoBlock
from basal.remote_value import RemoteValue

class Plan():
    def __init__(self) -> None:
        self.logging = Logging("Plan")

        self.meteo = MeteoBlock()

        RemoteValue.add("test.meteo.temperature", self.meteo.temperature)

        RemoteValue.add("test.meteo.pressure", self.meteo.pressure)
        RemoteValue.add("test.meteo.humidity", self.meteo.humidity)

Plan()
