#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from ___block_types import BlockTypes
from ___extended_block_base import BlockWithOneExtension
from ___active_variable import ActiveVariable
from micropython import const
from ___vl53l1x import VL53L1X

#_charging_state_command = const(0x01)

class DistanceBlock(BlockWithOneExtension):

  def __init__(self, address: int=None, measurement_period: float=1):
    """
    @param address:block address
    @param mesurement_period: sampling frequency in sec
    """
    super().__init__(BlockTypes.distance, address)
    #self.is_usb_connected = ActiveVariable(False, measurement_period, self._get_usb_state)
    self._vl53 = VL53L1X(self.i2c, 0x29)
    self.distance = ActiveVariable(0, measurement_period, self._get_distance)

    #self._vl53_init()


  def _get_distance(self):
    data = self._vl53.read()
    #print(tof.read())
    return data

  #def _vl53_init(self):
  #  self._vl53.set_Vcsel_pulse_period(self._vl53.vcsel_period_type[0], 18)
  #  self._vl53.set_Vcsel_pulse_period(self._vl53.vcsel_period_type[1], 14)




