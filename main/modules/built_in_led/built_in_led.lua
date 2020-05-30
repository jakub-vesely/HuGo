--[[
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available on https://opensource.org/licenses/MIT
--]]

local built_in_led = {}
function built_in_led.change_state(on)
    return c_built_in_led_change_state(on);
end

return built_in_led
