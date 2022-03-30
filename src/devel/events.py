from ___basal.___logging import Logging
from ___blocks.___sound_block import SoundBlock

from ___remote_control.___remote_key import RemoteKey
from ___remote_control.___special_keys import SpecialKeys
from ___blocks.___ir_block import IrBlock
from ___remote_control.___ir_numeric_remote import IrNumericRemote
from ___basal.___ble import Ble

class Plan():
  volume_step = 4

  def __init__(self) -> None:
    self.logging = Logging("events")
    self.sound = SoundBlock()
    self.volume = self.sound.get_volume()

    self.ir_block = IrBlock()
    self.ir_block.add_remote(IrNumericRemote())
    self.ir_block.value.updated(True, self.process_remote, self.ir_block.value)
    Ble.value_remote.updated(True, self.process_remote, Ble.value_remote)

  def process_remote(self, active_value):
    remote_key: RemoteKey = active_value.get()
    if remote_key == RemoteKey.get_default():
      return
    self.logging.info("pressed: ", remote_key)
    if not remote_key or remote_key.name == "":
      return
    remote_name = remote_key.name
    self.logging.info("pressed: ", remote_name)

    if remote_name == SpecialKeys.left:
      self.sound.play_previous()

    elif remote_name == SpecialKeys.right:
      self.sound.play_next()

    elif remote_name == SpecialKeys.up:
      self.volume += self.volume_step
      if self.volume > self.sound.get_max_volume():
        self.volume = self.sound.get_max_volume()
      self.logging.info("volume increased to: %d", self.volume)
      self.sound.set_volume(self.volume)

    elif remote_name == SpecialKeys.down:
      self.volume -= self.volume_step
      if self.volume < self.sound.get_min_volume():
        self.volume = self.sound.get_min_volume()
      self.logging.info("volume decreased to: %d", self.volume)
      self.sound.set_volume(self.volume)

    elif remote_name in (SpecialKeys.space, SpecialKeys.ok):
      self.sound.play_pause()

    elif remote_name.isdigit():
      key_number = int(remote_name)
      if key_number == 0:
        self.sound.stop()
      else:
        self.sound.play_by_index(key_number)

Plan()
