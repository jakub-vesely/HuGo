#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from ___basal.___logging import Logging
from ___blocks.___meteo_block import MeteoBlock
from ___blocks.___display_block import DisplayBlock
from ___basal.___remote_value import RemoteValue

class Plan():
    def __init__(self) -> None:
        self.logging = Logging("Plan")
        self.meteo = MeteoBlock()
        self.meteo.temperature.changed(self.value_changed)
        self.meteo.pressure.changed(self.value_changed)
        self.meteo.humidity.changed(self.value_changed)
        self.first_values_update_listener =  self.meteo.value.updated(self.first_update)

        RemoteValue.add("meteo.temperature", self.meteo.temperature)
        RemoteValue.add("meteo.pressure", self.meteo.pressure)
        RemoteValue.add("meteo.humidity", self.meteo.humidity)

        self.display = DisplayBlock()
        self.display.contrast(1)

    def first_update(self):
        self.meteo.value._remove_listener(self.first_values_update_listener)
        self.value_changed()

    def value_changed(self):
        self.logging.info(str(self.meteo.temperature) + ", " + str(self.meteo.pressure) + ", " + str(self.meteo.humidity))
        self.display.clean()
        self.display.print_text(1, 1,  self.meteo.temperature.get_str_to_fit(7))
        self.display.print_text(1, 10, self.meteo.pressure.get_str_to_fit(7))
        self.display.print_text(1, 20, self.meteo.humidity.get_str_to_fit(7))
        self.display.showtime()
Plan()
