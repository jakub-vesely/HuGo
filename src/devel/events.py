from ___basal.___ble import Ble
from ___basal.___logging import Logging
from ___basal.___planner import Planner
from ___blocks.___sound_block import SoundBlock, Status, PlayMode, EqMode
from ___blocks.___display_block import DisplayBlock
from ___blocks.___ir_block import IrBlock
from ___remote_control.___remote_key import RemoteKey
from ___remote_control.___special_keys import SpecialKeys
from ___remote_control.___ir_numeric_remote import IrNumericRemote

class Plan():
  volume_step = 4
  def __init__(self) -> None:
    self.logging = Logging("events")
    Planner.repeat(1, self.print_text)


  def print_text(self):
    self.logging.info("ahoj svete")

Plan()
