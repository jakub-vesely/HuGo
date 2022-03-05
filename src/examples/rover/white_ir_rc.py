from ir_rc_base import IrRcBase
class WhiteIrRc(IrRcBase):
  address = 59969
  key_3000 = 16
  key_6000 = 17
  key_plus = 18
  key_minus = 19
  key_0 = 20
  key_1 = 21
  key_moon = 65

  @classmethod
  def _get_address(cls):
    return cls.address
