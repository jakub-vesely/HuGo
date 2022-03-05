from ___logging import Logging
from ___ir_block import IrBlock
from remote_control.___ir_numeric_remote import IrNumericRemote
from remote_control.___remote_key import RemoteKey
from ___rgb_led_block import RgbLedBlock
from ___ble import Ble

class Plan():
  def __init__(self) -> None:
    self.logging = Logging("events")
    self.rgb = RgbLedBlock()

    self.ir_block = IrBlock()
    self.ir_block.add_remote(IrNumericRemote())

    #to demonstrate equal to
    self.ir_block.value.equal_to(RemoteKey("0"), True, self.light_off)
    Ble.value_remote.equal_to(RemoteKey("0"), True, self.light_off)
    #to demonstrate changed
    self.ir_block.value.updated(True, self.changed, self.ir_block.value)
    Ble.value_remote.updated(True, self.changed, Ble.value_remote)

  def changed(self, active_value):
    value = active_value.get()
    self.logging.info(value.name)
    if value and value.name.isdigit():
      key_number = int(value.name)
      if key_number > 0 and key_number < 10:
        self.rgb.set_color_by_id(key_number)
        self.logging.info("set color with id:%d", key_number)

  def light_off(self):
    self.rgb.set_off()
    self.logging.info("light turned off")

Plan()
