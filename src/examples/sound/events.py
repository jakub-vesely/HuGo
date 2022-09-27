#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from basal.ble import Ble
from basal.logging import Logging
from basal.planner import Planner
from blocks.sound_block import SoundBlock, Status, PlayMode, EqMode
from blocks.display_block import DisplayBlock
from blocks.ir_block import IrBlock
from remote_control.remote_key import RemoteKey
from remote_control.special_keys import SpecialKeys
from remote_control.ir_numeric_remote import IrNumericRemote

class Plan():
  volume_step = 4
  def __init__(self) -> None:
    self.dir_name = "test"
    self.logging = Logging("events")
    self.sound = SoundBlock()
    self.volume = self.sound.get_volume()
    self.dir_count = self.sound.get_folder_track_count(self.dir_name)
    self.display = DisplayBlock()
    Planner.repeat(0.2, self.fill_display)

    self.ir_block = IrBlock()
    self.ir_block.add_remote(IrNumericRemote())
    self.ir_block.value.updated(self.process_remote, self.ir_block.value)
    Ble.value_remote.updated(self.process_remote, Ble.value_remote)
    self.sound.play_by_folder_name("test", "04Znouze")

  def _get_status(self):
    status = self.sound.get_status()
    if status == Status.stop:
      return "stopped"
    if status == Status.play:
      return "playing"
    if status == Status.pause:
      return "paused"

  def _get_play_mode(self):
    play_mode = self.sound.get_play_mode()
    if play_mode == PlayMode.all_track_loop_play:
      return "all_loop"
    if play_mode == PlayMode.random_playing:
      return "random"
    if play_mode == PlayMode.single_loop_play:
      return "one_loop"
    if play_mode == PlayMode.single_play:
      return "single"

  def _get_eq_mode(self):
    eq_mode = self.sound.get_eq_mode()
    if eq_mode == EqMode.base:
      return "base"
    if eq_mode == EqMode.classinc:
      return "classic"
    if eq_mode == EqMode.jazz:
      return "jazz"
    if eq_mode == EqMode.normal:
      return "normal"
    if eq_mode == EqMode.pop:
      return "pop"
    if eq_mode == EqMode.rock:
      return "rock"

  def fill_display(self):
    self.display.clean()
    self.display.print_text(0, 0, self.sound.get_track_name())
    self.display.print_text(0, 8, self._get_status())
    self.display.print_text(0, 16, self._get_play_mode())
    self.display.print_text(0, 24, self._get_eq_mode())
    self.display.print_text(0, 32, str(self.sound.get_volume()))
    self.display.showtime()

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

    elif remote_name == "*":
      self.sound.toggle_play_mode()

    elif remote_name == "#":
      self.sound.toggle_eq_mode()

    elif remote_name.isdigit():
      key_number = int(remote_name)
      if key_number == 0:
        self.sound.stop()
      else:
        self.sound.play_by_index(key_number)

Plan()
