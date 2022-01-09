from ___logging import Logging
from ___power_mgmt import PowerMgmt
import sys
import time
import uasyncio

logging = Logging("planner")
unhandled_exception_prefix = "Unhandled exception"


class TaskProperties:
  kill = False
  waiting_start_ms = 0
  waiting_time_ms = 0

_performed_tasks = {} #expected dict with task handle as key and tasks properties as value
_handle_count = 0
_loop = uasyncio.get_event_loop()
_power_mgmt = PowerMgmt()

def _get_next_handle():
  global _handle_count
  handle = _handle_count
  _handle_count += 1
  return handle

async def _async_plan(handle, function, *args, **kwargs):
  if not _performed_tasks[handle].kill:
    try:
      function(*args, **kwargs)
    except Exception as error:
      logging.exception(error, extra_message=unhandled_exception_prefix)
  del _performed_tasks[handle]

def plan(function, *args, **kwargs):
  handle = _get_next_handle()
  _performed_tasks[handle] = TaskProperties()
  _loop.create_task(_async_plan(handle, function, *args, **kwargs))
  return handle

async def _task_sleep(handle, delay_s):
  task = _performed_tasks[handle]
  task.waiting_start_ms = time.ticks_ms()
  task.waiting_time_ms = int(delay_s * 1000)
  while time.ticks_ms() < task.waiting_start_ms + task.waiting_time_ms and not task.kill:
    await uasyncio.sleep_ms(0) # give chance another task

  task.waiting_time_ms = 0

async def _async_postpone(handle, delay_s, function, *args, **kwargs):
  await _task_sleep(handle, delay_s)
  if not _performed_tasks[handle].kill:
    try:
      function(*args, **kwargs)
    except Exception as error:
      logging.exception(error, extra_message=unhandled_exception_prefix)
  del _performed_tasks[handle]

def postpone(delay_s, function, *args, **kwargs):
  handle = _get_next_handle()
  _performed_tasks[handle] = TaskProperties()
  _loop.create_task(_async_postpone(handle, delay_s, function, *args, **kwargs))
  return handle

async def _async_repeat(handle, delay_s, function, *args, **kwargs):
  if not _performed_tasks[handle].kill:
    try:
      function(*args, **kwargs)
    except Exception as error:
      logging.exception(error, extra_message=unhandled_exception_prefix)
    await _task_sleep(handle, delay_s) # in case of kill task is created and the kill feature is tested when is executed
    _loop.create_task(_async_repeat(handle, delay_s, function, *args, **kwargs))
  else:
    del _performed_tasks[handle]

def repeat(delay_s, function, *args, **kwargs):
  handle = _get_next_handle()
  _performed_tasks[handle] = TaskProperties()
  _loop.create_task(_async_repeat(handle, delay_s, function, *args, **kwargs))
  return handle

async def _async_main():
  while True:
    now = time.ticks_ms()
    remains_min = sys.maxsize
    for properties in _performed_tasks.values():
      if properties.kill or properties.waiting_time_ms == 0: #tasks dedicated to be killed and tasks that do not have set any waiting are skipped
        continue

      remains = properties.waiting_time_ms - (now - properties.waiting_start_ms)
      if remains < remains_min:
        remains_min = remains

    if remains_min == sys.maxsize or remains_min <= 0:
      await uasyncio.sleep_ms(0)
    elif not _power_mgmt.is_power_save_enabled() or remains_min < 100:
      await uasyncio.sleep_ms(remains_min)
    else:
      #print("light_sleep for %d ms" % remains_min)
      _power_mgmt.light_sleep(remains_min)

def run():
  _loop.run_until_complete(_async_main())

def kill_task(handle):
  if handle in _performed_tasks:
    _performed_tasks[handle].kill = True
