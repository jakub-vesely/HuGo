--[[
Copyright (c) 2021 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]

local TinyBlockBase = {}
TinyBlockBase.__index = TinyBlockBase
-- TinyBlockBase.address = nil


function TinyBlockBase:check_address_type(address)
    assert(type(address) == "number", "tiny-block address is not set as a number")
end

function TinyBlockBase:new(address)
    o = setmetatable({}, TinyBlockBase)
    TinyBlockBase:check_address_type(address)
    o.address = address
    return o
end

function TinyBlockBase:get_i2c_address()
    return self.address
end

function TinyBlockBase:set_new_i2c_address(new_address)
    self:check_address_type(new_address)
    cl_tiny_base_set_i2c_address(self.address, new_address)
    print("I2C addres changing from", self.address, "to", new_address)
    self.address = new_address
end

return TinyBlockBase


