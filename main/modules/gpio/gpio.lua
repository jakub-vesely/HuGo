--[[
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available on https://opensource.org/licenses/MIT
--]]

local gpio =  {}

function gpio.init_pin_for_out(pin)
    return c_init_pin_for_out(pin)
end

function gpio.set_logic_value(pin, value)
    return c_set_logic_value(pin, value)
end

function gpio.init_pin_for_pwm(pin, frequency, duty)
    return c_init_pin_for_pwm(pin, frequency, duty)
end

function gpio.set_pwm_frequency(frequency)
    return c_set_pwm_frequency(frequency)
end

function gpio.set_pwm_duty(duty)
    return c_set_pwm_duty(duty)
end

return gpio
