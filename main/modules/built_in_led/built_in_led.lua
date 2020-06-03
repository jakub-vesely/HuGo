--[[
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available on https://opensource.org/licenses/MIT
--]]

local build_in_led = {}
function build_in_led.change_state(on)
    return cl_built_in_led_change_state(on);
end

return build_in_led
