--[[
    Copyright (c) 2020 jakub-vesely
    This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]

--local gpio = require "gpio"
-- local luvent = require "Luvent"
-- local built_in_led = require("built_in_led")
local timer = require("timer")
local chassis = require("chassis")

-- local program_started = luvent.newEvent()
-- local led_state = 1

-- local function connect()
--     program_started:addAction(
--         function ()
--             print("action")
--             built_in_led.change_state(1)
--         end
--     )
-- end

-- connect()
-- program_started:trigger()

-- function After_time()
--     if led_state == 0 then
--         led_state = 1
--     else
--         led_state = 0
--     end
--     built_in_led.change_state(led_state)
--     timer.call_after_time(After_time, 0.5)
-- end

-- timer.call_after_time(After_time, 3)

-- local pwm_duty = 0
-- Pwm_change = function()
--     pwm_duty = pwm_duty + 1
--     if pwm_duty > 10 then
--         pwm_duty = 0
--     end
--     gpio.set_pwm_duty(pwm_duty * 10)
--     timer.call_after_time(Pwm_change, 0.3)
-- end
-- gpio.init_pin_for_pwm(12, 100, pwm_duty)
-- timer.call_after_time(Pwm_change, 0.3)

-- local rotate = 0
-- function ChangeDirection()
--     if rotate == 1 then
--         rotate = 0
--         gpio.set_logic_value(32, 0)
--         gpio.set_logic_value(33, 1)
--     else
--         rotate = 1
--         gpio.set_logic_value(32, 1)
--         gpio.set_logic_value(33, 0)
--     end
--     timer.call_after_time(ChangeDirection, 1)
-- end

function Right()
    chassis.rotate_counterclockwise()
    timer.call_after_time(Stop, 1)
end


function Backward()
    chassis.go_backward()
    timer.call_after_time(Right, 1)
end

function Left()
    chassis.rotate_clockwise()
    timer.call_after_time(Backward, 1)
end

function Forward()
    chassis.go_forward()
    timer.call_after_time(Left, 1)
end

function Stop()
    chassis.stop()
    timer.call_after_time(Forward, 1)
end
timer.call_after_time(Stop, 1)

-- gpio.init_pin_for_out(25)
-- gpio.init_pin_for_out(26)
-- gpio.set_logic_value(25, 0)
-- gpio.set_logic_value(26, 0)

-- gpio.init_pin_for_out(32)
-- gpio.init_pin_for_out(33)
-- gpio.set_logic_value(32, 1)
-- gpio.set_logic_value(33, 0)
-- timer.call_after_time(ChangeDirection, 1)

--main loop
while true do
    c_task_delay() -- must be in main loop to be called watchdog
end
return 1
