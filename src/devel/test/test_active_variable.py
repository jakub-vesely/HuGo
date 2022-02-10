import asyncio
from unittest.mock import MagicMock
from hugo_test_case import TestCase #must be called before another imports from devel or micropython - redefine import
from ___active_variable import ActiveVariable
from ___planner import Planner

class TestActiveVariable(TestCase):
  def setUp(self):
    self.next_value = 10
    self.var = ActiveVariable(renew_func=self._get_value)
    self.called_func = MagicMock()

  def _get_value(self):
    return self.next_value

  def _test_called_once(self):
    self.var.get_value(True)
    self.called_func.assert_called_once()

  def _test_not_called(self):
    self.var.get_value(True)
    self.called_func.assert_not_called()

  def test_equal_to__int_true(self):
    self.var.equal_to(self.next_value, False, self.called_func)
    self._test_called_once()

  def test_equal_to__int_false(self):
    self.var.equal_to(self.next_value + 1, False, self.called_func)
    self._test_not_called()

  def test_equal_to__float_true(self):
    self.var.equal_to(float(self.next_value), False, self.called_func)
    self._test_called_once()

  def test_equal_to__float_false(self):
    self.var.equal_to(float(self.next_value) + 0.1, False, self.called_func)
    self._test_not_called()

  def test_not_equal_to__float_true(self):
    self.var.not_equal_to(float(self.next_value) + 0.1, False, self.called_func)
    self._test_called_once()

  def test_not_equal_to__float_false(self):
    self.var.not_equal_to(float(self.next_value), False, self.called_func)
    self._test_not_called()

  def test_less_than__true(self):
    self.var.less_than(self.next_value + 1, False, self.called_func)
    self._test_called_once()

  def test_less_than__false(self):
    self.var.less_than(self.next_value, False, self.called_func)
    self._test_not_called()

  def test_more_than__true(self):
    self.var.more_than(self.next_value - 1, False, self.called_func)
    self._test_called_once()

  def test_more_than__false(self):
    self.var.more_than(self.next_value, False, self.called_func)
    self._test_not_called()

  def test_in_range__true(self):
    self.var.in_range(self.next_value, self.next_value + 1, False, self.called_func)
    self._test_called_once()

  def test_in_range__false(self):
    self.var.in_range(self.next_value + 1, self.next_value + 2,  False, self.called_func)
    self._test_not_called()

    self.var.in_range(self.next_value - 1, self.next_value,  False, self.called_func)
    self._test_not_called()

  def test_out_of_range__true(self):
    self.var.out_of_range(self.next_value + 1, self.next_value + 2,  False, self.called_func)
    self._test_called_once()

    self.var.out_of_range(self.next_value - 1, self.next_value,  False, self.called_func)
    self._test_called_once()

  def test_out_of_range__false(self):
    self.var.out_of_range(self.next_value, self.next_value + 1, False, self.called_func)
    self._test_not_called()

  def test_value_changed__true(self):
    self.var.changed(False, self.called_func)
    self._test_called_once()

    #call set with the same value should not be added another call
    self.var.changed(False, self.called_func)
    self._test_called_once()

  def test_value_updated(self):
    self.var.updated(False, self.called_func)
    self._test_called_once()

  def test_set_get_value(self):
    self.var.set_value(self.next_value)
    self.assertEqual(self.var.get_value(), self.next_value)

  def test__get_value__force(self):
    self.assertEqual(self.var.get_value(force=True), self.next_value)

  def test_remove_trigger(self):
    handler = self.var.equal_to(self.next_value, False, self.called_func)
    self.var.remove_trigger(handler)
    self._test_not_called()

  # TODO:need to be finished virtualization of micropython libraries first
  # async def _test_change_period(self):
  #   self.var.equal_to(self.next_value, False, self.called_func)
  #   self.var.change_period(0.1)
  #   await asyncio.sleep(1)
  #   self.called_func.assert_called_once()
  # def test_change_period(self):
  #   Planner.run()
  #   asyncio.run(self._test_change_period())







