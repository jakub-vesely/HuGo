--[[
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]

local Rgb = {}
Rgb.address = nil
function Rgb:new(address)
    assert(type(address) == "number", "tiny-block address is not set as a number")
    o = {}
    o.address = address
    setmetatable(o, self)
    self.__index = self
    return o
end

function Rgb:set_values(red, green, blue)
    cl_rgb_set_values(self.address, red, green, blue)
end

return Rgb