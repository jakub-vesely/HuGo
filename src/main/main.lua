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
local block_factory = require "block_factory"
local timer = require("timer")

local display = block_factory.display_block(0x02)
display:print_text_8x8(0, 0, 64, 48, "A1A2A3a4\nB1B2B3b\nC1C2C3c\nD1D2D3d\nE1E2E3e\nF1F2F3f4", true)
display:showtime()
local counter = 0;

function display_counter()
    display:clean()
    local text = tostring(counter)
    print(text)
    display:print_text_8x8(0, 0, 64, 48, text, true)
    display:showtime()
    counter = counter + 1
    timer.call_after_time(display_counter, 1)
end
display_counter()

--display:set_new_i2c_address(0x06)

-- display.clean()
-- print("disp cleaned")
-- display.print_text_8x16(0, 0, 64, 32, "hallo", true)
-- print("disp text")
-- display.showtime()
-- print("disp shown")
-- local mpu = require("mpu9250")
-- local chassis = require("chassis")

-- mpu.add_measured_action(
--     function()
--         local value_accel_y = string.format("accelY: %4.2f", mpu.get_accel_y())
--         local value_gyro_z = string.format("gyroZ: %4.2f", mpu.get_gyro_z())
--         display.clean()
--         display.print_text_8x16(0, 0, 64, 32, value_accel_x, true)
--         display.print_text_8x16(0, 0, 128, 32, value_accel_y, true)
--         display.print_text_8x16(0, 16, 128, 32, value_gyro_z, true)
--         display.showtime()
--         -- print("value_accel_y %s", value_accel_y)
--         if mpu.get_accel_y()< -1 then
--             chassis.stop()
--         end
--     end
-- )

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


-- local rgb = block_factory.rgb_led_block(0x04)
-- rgb:set_new_i2c_address(0x04)
-- --local counter = 0;
-- --local reverse = false;

-- function set_on()
--     rgb:set_on()
--     timer.call_after_time(set_off, 1)
-- end

-- function set_off()
--     rgb:set_off()
--     timer.call_after_time(set_red, 1)
-- end

-- set_on()

-- function set_red()
--      rgb:set_color(rgb.color.red)
--      timer.call_after_time(set_green, 1)
-- end
-- function set_green()
--     rgb:set_color(rgb.color.green)
--     timer.call_after_time(set_red, 1)
-- end
-- set_red()

-- function set_blue()
--     rgb:set_color(rgb.color.blue)
--     timer.call_after_time(set_on, 1)
-- end
-- set_red()

    -- function led_change()
--     rgb:set_values(255 - counter, counter, 0)

--     if reverse then
--         if counter == 0 then
--             reverse = false
--             counter = 1
--         else
--             counter = counter - 1
--         end
--     else
--         if counter == 255 then
--             reverse = true
--             counter = 255
--         else
--             counter = counter + 1
--         end
--     end

--     timer.call_after_time(led_change, 0.1)
-- end


-- timer.call_after_time(led_change, 1)



--
-- local motor_driver_block = block_factory.motor_driver_block(0x02)
-- --motor_driver_block:set_new_i2c_address(0x05)

-- function start_motor()
--     local count1 = motor_driver_block:get_counter(1)
--     local count2 = motor_driver_block:get_counter(2)
--     print("count1", count1, "count2", count2)

--     print(count1)
--     if count1 < 1000 then
--         motor_driver_block:speed_100(1)
--         motor_driver_block:speed_100(2)
--         timer.call_after_time(stop_motor, 1)
--         timer.call_after_time(stop_motor, 2)
--     end

-- end

-- function stop_motor()
--     motor_driver_block:speed_0(1)
--     timer.call_after_time(start_motor, 1)
-- end

-- --timer.call_after_time(start_motor, 1)
-- motor_driver_block:power_on(1)
-- motor_driver_block:power_on(2)
-- motor_driver_block:turn_clockwise(1)
-- motor_driver_block:turn_anticlockwise(2)
-- -- motor_driver_block:speed_100(1)
-- -- motor_driver_block:speed_100(2)
-- start_motor()


-- local power_block = block_factory.power_block(0x03)
-- --power_block:set_new_i2c_address(0x03)
-- local function charging()
--     if (power_block:is_charging()) then
--         rgb:set_color(rgb.color.red)
--     else
--         rgb:set_color(rgb.color.green)
--     end
--     timer.call_after_time(charging, 0.5)
-- end
-- charging()

--

-- local function charging()
--     print("charging", power_block:is_charging())
--     print("voltage", power_block:get_voltage())
--     print("current", power_block:get_current_ma())
--     timer.call_after_time(charging, 1)
-- end

--charging()

--print("i2c_address", power_block:get_i2c_address())
--print("power ina address", power_block:get_ina_i2c_address())

--power_block:set_ina_i2c_address(power_block.InaAddresses.INA_I2C_0x45)

--print("power ina address", power_block:get_ina_i2c_address())

--power_block:set_new_i2c_address(0x04)
--print("power address0", power_block:get_ina_i2c_address())
-- power_block:set_ina_a0a1(1)
-- print("power address1", power_block:get_ina_i2c_address())
-- power_block:set_ina_a0a1(2)
-- print("power address2", power_block:get_ina_i2c_address())
-- power_block:set_ina_a0a1(3)
-- print("power address3", power_block:get_ina_i2c_address())


-- local ir = block_factory.ir_receiver_block(0x02)
-- -- ir:set_new_i2c_address(0x06)

-- function get_ir()
--     local ir_data = ir:get_ir_data();
--     if (ir_data ~= 0) then
--         print("ir_data", ir_data)
--     end
--     timer.call_after_time(get_ir, 1)
-- end
-- get_ir()
print("behavior definition finished")
