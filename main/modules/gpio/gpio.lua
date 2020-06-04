--[[
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]

local gpio =  {}

function gpio.set_pin_for_out(pin)
    return cl_gpio_set_pin_for_out(pin)
end

function gpio.set_logic_value(pin, value)
    return cl_gpio_set_pin_logic_value(pin, value)
end

function gpio.set_pin_for_pwm(pin, frequency, duty)
    return cl_gpio_set_pin_for_pwm(pin, frequency, duty)
end

function gpio.set_pwm_frequency(frequency)
    return cl_gpio_set_pwm_frequency(frequency)
end

function gpio.set_pwm_duty(duty)
    return cl_gpio_set_pwm_duty(duty)
end

return gpio
