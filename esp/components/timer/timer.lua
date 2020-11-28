--[[
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]

local timer =  {
    callbacks = {}
}

function timer.call_after_time(callback_function, time_in_sec)
    local timer_id = cl_create_new_timer()
    timer.callbacks[timer_id] = callback_function
    return cl_start_timer(timer_id, time_in_sec * 1000000);
end

function Timer_callback_collector(id)
    timer.callbacks[id]()
    timer.callbacks[id] = nil
end

return timer
