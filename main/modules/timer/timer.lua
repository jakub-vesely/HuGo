--[[
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]

local timer =  {
    callbacks = {},
    last_id = 0
}

function timer.call_after_time(callback_function, time_in_sec)
    timer.last_id = timer.last_id + 1
    timer.callbacks[timer.last_id] = callback_function
    cl_timer_call_after(timer.last_id, time_in_sec * 1000000);
end

function Timer_callback_collector(id)
    timer.callbacks[id]()
    timer.callbacks[id] = nil
end

return timer
