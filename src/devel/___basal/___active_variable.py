#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from ___basal.___planner import Planner
from ___basal.___logging import Logging
import math

class Conditions():
  equal_to = 1
  not_equal_to = 2
  less_than = 3
  more_than = 4
  in_range = 5
  out_of_range = 6
  value_changed = 7
  value_updated = 8

class ActiveVariable():
  logging = Logging("act_var")
  def __init__(self, initial_value=None, renew_period:float=0, renew_func=None, ignore_same=True):
    """
    @param initial_value: if is set Active variable will be preset to this value
    @param renew_period: renew_func will be called with this period if this value > 0
    @param renew_func: this method will be called periodically if renew_period is > 0 or if get is called with the parameter "force"
    @param ignore_same: does not call listener function when value does not change although the condition is met". Value updated listener is executed anyway.

    """
    self._old_value = initial_value
    self._value = initial_value
    self._renew_period = renew_period
    self._renew_func = renew_func
    self._ignore_same = ignore_same
    self._renew_handle = None
    self._listeners = list()
    self._handle_count = 0

  def change_period(self, new_period):
    self._renew_period = new_period
    if self._renew_handle:
      Planner.kill_task(self._renew_handle)
      self._renew_handle = None
    if self._renew_period > 0:
      self._renew_handle = Planner.repeat(self._renew_period, self._update_value)

  def set(self, value):
    if value is None:
      return #nothing to compare

    same = False
    if self._ignore_same:
      if isinstance(self._value, float) and isinstance(value, float):
        if math.isclose(self._value, value):
          same = True
      else:
        if self._value == value:
          same = True

    self._old_value = self._value
    self._value = value

    for listener in self._listeners:
      _type = listener[1]
      repeat = listener[2]
      if _type == Conditions.value_updated:
        if  not repeat:
          self._remove_listener(listener)
        listener[3](*listener[4], **listener[5])
      elif not same:
        if _type == Conditions.equal_to:
          if isinstance(value, float) or isinstance(listener[3], float):
            if math.isclose(value, listener[3]):
              if  not repeat:
                self._remove_listener(listener) #TODO: maybe it can be returned to the end of this method it should not be important for the called methd that it will be removed later - events are synchronous
              listener[4](*listener[5], **listener[6])
          else:
            if value == listener[3]:
              if  not repeat:
                self._remove_listener(listener)
              listener[4](*listener[5], **listener[6])
        elif _type == Conditions.not_equal_to:
          if isinstance(value, float) or isinstance(listener[3], float):
            if not math.isclose(value, listener[3]):
              if  not repeat:
                self._remove_listener(listener)
              listener[4](*listener[5], **listener[6])
          else:
            if value != listener[3]:
              if  not repeat:
                self._remove_listener(listener)
              listener[4](*listener[5], **listener[6])
        elif _type == Conditions.less_than:
          if value < listener[3]:
            if  not repeat:
              self._remove_listener(listener)
            listener[4](*listener[5], **listener[6])
        elif _type == Conditions.more_than:
          if value > listener[3]:
            if  not repeat:
              self._remove_listener(listener)
            listener[4](*listener[5], **listener[6])
        elif _type == Conditions.in_range:
          if value >= listener[3] and value < listener[4]:
            if  not repeat:
              self._remove_listener(listener)
            listener[5](*listener[6], **listener[7])
        elif _type == Conditions.out_of_range:
          if value < listener[3] or value >= listener[4]:
            if  not repeat:
              self._remove_listener(listener)
            listener[5](*listener[6], **listener[7])
        elif _type == Conditions.value_changed:
          if  not repeat:
            self._remove_listener(listener)
          listener[3](*listener[4], **listener[5])
        else:
          self.logging.error("unknown listener type %s" % str(listener[1]))

  def get(self, force=False):
    if force:
      self._update_value()
    return self._value

  def get_previous_value(self):
    return self._old_value

  def _update_value(self):
    if self._renew_func:
      self.set(self._renew_func())

  def _add_listener(self, listener):
    if not self._listeners and self._renew_period > 0:
      self._renew_handle = Planner.repeat(self._renew_period, self._update_value)
    self._listeners.append(listener)
    self._handle_count += 1
    return listener[0] #returns provided handle

  def remove_trigger(self, handle):
    for listener in self._listeners:
      if listener[0] == handle:
        self._listeners.remove(listener)
        if not self._listeners:
          Planner.kill_task(self._renew_handle)
          self._renew_handle = None
        return True
    return False

  def _remove_listener(self, listener):
    return self.remove_trigger(listener[0])

  def equal_to(self, expected, function: callable, *args, **kwargs):
    """
    provided function with arguments will be called when
    measured value is equal to the expected value
    provided function will be called repeatedly until returned listener is not removed
    """
    return self._add_listener((self._handle_count, Conditions.equal_to, True, expected, function, args, kwargs))

  def equal_to_once(self, expected, function: callable, *args, **kwargs):
    """
    provided function with arguments will be called when
    measured value is equal to the expected value
    provided function will be called only once
    """
    return self._add_listener((self._handle_count, Conditions.equal_to, False, expected, function, args, kwargs))


  def not_equal_to(self, expected, function: callable, *args, **kwargs):
    """
    provided function with arguments will be called when
    measured value is not equal to the expected value
    provided function will be called repeatedly until returned listener is not removed
    """
    return self._add_listener((self._handle_count, Conditions.not_equal_to, True, expected, function, args, kwargs))

  def not_equal_to_once(self, expected, function: callable, *args, **kwargs):
    """
    provided function with arguments will be called when
    measured value is not equal to the expected value
    provided function will be called only once
    """
    return self._add_listener((self._handle_count, Conditions.not_equal_to, False, expected, function, args, kwargs))


  def less_than(self, expected, function: callable, *args, **kwargs):
    """
    provided function with arguments will be called when
    measured value is smaller than the expected value
    provided function will be called only repeatedly until returned listener is not removed
    """
    return self._add_listener((self._handle_count, Conditions.less_than, True, expected, function, args, kwargs))

  def less_than_once(self, expected, function: callable, *args, **kwargs):
    """
    provided function with arguments will be called when
    measured value is smaller than the expected value
    provided function will be called only once
    """
    return self._add_listener((self._handle_count, Conditions.less_than, False, expected, function, args, kwargs))


  def more_than(self, expected, function: callable, *args, **kwargs):
    """
    provided function with arguments will be called when
    measured value is bigger than the expected value
    provided function will be called only repeatedly until returned listener is not removed
    """
    return self._add_listener((self._handle_count, Conditions.more_than, True, expected, function, args, kwargs))

  def more_than_once(self, expected, function: callable, *args, **kwargs):
    """
    provided function with arguments will be called when
    measured value is bigger than the expected value
    provided function will be called only once
    """
    return self._add_listener((self._handle_count, Conditions.more_than, False, expected, function, args, kwargs))


  def in_range(self, expected_begin, expected_end, function: callable, *args, **kwargs):
    """
    provided function with arguments will be called when
    measured value is bigger or equal to the expected_begin value and smaller that the expected_end value
    provided function will be called only repeatedly until returned listener is not removed
    """
    return self._add_listener((self._handle_count, Conditions.in_range, True, expected_begin, expected_end, function, args, kwargs))

  def in_range_once(self, expected_begin, expected_end, function: callable, *args, **kwargs):
    """
    provided function with arguments will be called when
    measured value is bigger or equal to the expected_begin value and smaller that the expected_end value
    provided function will be called only once
    """
    return self._add_listener((self._handle_count, Conditions.in_range, False, expected_begin, expected_end, function, args, kwargs))


  def out_of_range(self, expected_begin, expected_end, function: callable, *args, **kwargs):
    """
    provided function with arguments will be called when
    measured value is smaller than the expected_begin value or bigger or equal to the expected_end value
    provided function will be called only repeatedly until returned listener is not removed
    """
    return self._add_listener((self._handle_count, Conditions.out_of_range, True, expected_begin, expected_end, function, args, kwargs))

  def out_of_range_once(self, expected_begin, expected_end, function: callable, *args, **kwargs):
    """
    provided function with arguments will be called when
    measured value is smaller than the expected_begin value or bigger or equal to the expected_end value
    provided function will be called only once
    """
    return self._add_listener((self._handle_count, Conditions.out_of_range, False, expected_begin, expected_end, function, args, kwargs))

  def changed(self, function: callable, *args, **kwargs):
    """
    provided function with arguments will be called when
    measured value is different than last time measured value
    provided function will be called only repeatedly until returned listener is not removed
    """
    return self._add_listener((self._handle_count, Conditions.value_changed, True, function, args, kwargs))

  def changed_once(self, function: callable, *args, **kwargs):
    """
    provided function with arguments will be called when
    measured value is different than last time measured value
    provided function will be called only once
    """
    return self._add_listener((self._handle_count, Conditions.value_changed, False, function, args, kwargs))


  def updated(self, function: callable, *args, **kwargs):
    """
    provided function with arguments will be called always when
    a value is updated (when it is measured)
    provided function will be called only repeatedly until returned listener is not removed
    """
    return self._add_listener((self._handle_count, Conditions.value_updated, True, function, args, kwargs))

  def updated_once(self, function: callable, *args, **kwargs):
    """
    provided function with arguments will be called always when
    a value is updated (when it is measured)
    provided function will be called only once
    """
    return self._add_listener((self._handle_count, Conditions.value_updated, False, function, args, kwargs))

  def __str__(self):
    return str(self.get())
