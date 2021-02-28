--[[
    Copyright (c) 2020 jakub-vesely
    This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]

--local gpio = require "gpio"
--local chassis = require("chassis")

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

-- function Right()
--     chassis.rotate_counterclockwise()
--     timer.call_after_time(Stop, 1)
-- end

-- function Backward()
--     chassis.go_backward()
--     timer.call_after_time(Right, 1)
-- end

-- function Left()
--     chassis.rotate_clockwise()
--     timer.call_after_time(Backward, 1)
-- end

-- function Forward()
--     chassis.go_forward()
--     timer.call_after_time(Left, 1)
-- end

-- function Stop()
--     chassis.stop()
--     timer.call_after_time(Forward, 1)
-- end
-- timer.call_after_time(Stop, 1)

-- gpio.init_pin_for_out(25)
-- gpio.init_pin_for_out(26)
-- gpio.set_logic_value(25, 0)
-- gpio.set_logic_value(26, 0)

-- gpio.init_pin_for_out(32)
-- gpio.init_pin_for_out(33)
-- gpio.set_logic_value(32, 1)
-- gpio.set_logic_value(33, 0)
-- timer.call_after_time(ChangeDirection, 1)

print("behavior definition started")

-- local timer = require("timer")

-- local led_state = 1

-- function After_time()
--     if led_state == 0 then
--         led_state = 1
--     else
--         led_state = 0
--     end
--     print("led_state:", led_state)
--     --built_in_led.change_state(led_state)
--     timer.call_after_time(After_time, 0.5)
-- end

-- timer.call_after_time(After_time, 3)

-- local display = require("display")
-- local mpu = require("mpu9250")
-- local chassis = require("chassis")

-- mpu.add_measured_action(
--     function()
--         local value_accel_y = string.format("accelY: %4.2f", mpu.get_accel_y())
--         local value_gyro_z = string.format("gyroZ: %4.2f", mpu.get_gyro_z())
--         display.clean()
--         display.print_text_8x16(0, 0, 128, 32, value_accel_y, true)
--         display.print_text_8x16(0, 16, 128, 32, value_gyro_z, true)
--         display.showtime()
--         -- print("value_accel_y %s", value_accel_y)
--         if mpu.get_accel_y()< -1 then
--             chassis.stop()
--         end
--     end
-- )

local block_factory = require "block_factory"


local rgb = block_factory.rgb_led(0x02)

local counter = 0;
local reverse = false;
local timer = require("timer")

function led_change()
    rgb:set_values(255 - counter, counter, 0)

    if reverse then
        if counter == 0 then
            reverse = false
            counter = 1
        else
            counter = counter - 1
        end
    else
        if counter == 255 then
            reverse = true
            counter = 255
        else
            counter = counter + 1
        end
    end

    timer.call_after_time(led_change, 0.01)
end

timer.call_after_time(led_change, 0.01)

-- local ir_remote = require "ir_remote"
-- ir_remote.add_data_recoveved_action(
--     function (addr, code, repeated)
--         if repeated == 1 then
--             return
--         end
--         print("action", code)
--         if code == 59160 or code == 60690 then
--             print("forward")
--             chassis.go_forward()
--         elseif code == 44370 or code == 60435 then
--             print("back")
--             chassis.go_backward()
--         elseif code == 42330 or code == 61200 then
--             chassis.rotate_clockwise()
--         elseif code == 63240 or code == 60945 then
--             chassis.rotate_counterclockwise()
--         elseif code == 58140 or code == 46920 then
--             chassis.stop()
--         else
--             print("unknown command")
--         end
--     end
-- )
print("behavior definition finished")
