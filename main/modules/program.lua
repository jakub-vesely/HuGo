--[[
    Copyright (c) 2020 jakub-vesely
    This software is published under MIT license. Full text of the licence is available on https://opensource.org/licenses/MIT
--]]


local luvent = require "Luvent"
local build_in_led = require("interfcs.build_in_led_int")

local program_started = luvent.newEvent()

local function connect()
    program_started:addAction(
        function ()
            print("action")
            build_in_led.change_state(1)
        end
    )
end

connect()
program_started:trigger()
return 1
