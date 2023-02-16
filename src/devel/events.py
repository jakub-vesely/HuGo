#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from ___basal.___logging import Logging

from ___blocks.___meteo_block import MeteoBlock
from ___basal.___remote_value import RemoteValue

class Plan():
    def __init__(self) -> None:
        self.logging = Logging("Plan")

        self.meteo = MeteoBlock()

        RemoteValue.add("test.meteo.temperature", self.meteo.temperature)
        RemoteValue.add("test.meteo.pressure", self.meteo.pressure)
        RemoteValue.add("test.meteo.humidity", self.meteo.humidity)

Plan()
