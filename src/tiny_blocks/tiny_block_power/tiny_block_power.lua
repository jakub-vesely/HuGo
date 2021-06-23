--[[
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]
TinyBlockBase = require "tiny_block_base"
local Power = {}
Power.__index = Power

Power.InaAddresses = {
    INA_I2C_0x40 = 0,
    INA_I2C_0x41 = 1,
    INA_I2C_0x44 = 2,
    INA_I2C_0x45 = 3,
}

function Power:is_charging()
    return cl_power_is_charging(self.address)
end


function Power:_get_ina_i2c_address()
    return cl_power_get_ina_i2c_address(self.address)
end

function Power:get_ina_i2c_address()
    if self.ina_address == 0 then
        self.ina_address = self:_get_ina_i2c_address()
    end
    return self.ina_address
end

function Power:set_ina_i2c_address(ina_addresss)
    if ina_addresss < Power.InaAddresses.INA_I2C_0x40 or ina_addresss > Power.InaAddresses.INA_I2C_0x45  then
        error("unexpected ina_address")
    end
    cl_power_set_ina_a0a1(self.address, ina_addresss)
    self.ina_address = self:_get_ina_i2c_address()
end

function Power:initialize_ina()
    cl_power_initialize_ina(self:_get_ina_i2c_address())
end

function Power:get_voltage()
    return cl_power_get_voltage(self:get_ina_i2c_address())
end

function Power:get_current_ma()
    return cl_power_get_current_ma(self:get_ina_i2c_address())
end

function Power:new(address)
    local o = setmetatable(TinyBlockBase:new(address), Power)
    print("power address", o.address)
    o.ina_address = 0
    o:initialize_ina()
    return o
end
setmetatable(Power, {__index = TinyBlockBase})

return Power