--[[
Copyright (c) 2021 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]

TinyBlockBase = require "tiny_block_base"
local Display = {}
Display.__index = Display

Display.Addresses = {
    SSD_I2C_0x3C = 1,
    SSD_I2C_0x3D = 2,
}

function Display:get_ssd_i2c_dimensions()
    return cl_display_get_ssd_dimensions(self.address)
end

function Display:get_ssd_i2c_address()
    if self.ssd_address == 0 then
        self.ssd_address = self:_get_ina_i2c_address()
    end
    return self.ssd_address
end

function Display:showtime()
    return cl_display_showtime(self.address)
end

function Display:clean()
    return cl_display_clean(self.address)
end

function Display:draw_point(x, y, color)
    return cl_display_draw_point(self.address, x, y, color)
end

function Display:draw_elipse(x, y, r1, r2, color)
    return cl_display_draw_elipse(self.address, x, y, r1, r2, color)
end

function Display:print_text_8x8(x, y, r1, r2, text, color)
    return cl_display_print_text_8x8(self.address, x, y, r1, r2, text, color)
end

function Display:print_text_8x16(x, y, r1, r2, text, color)
    return cl_display_print_text_8x16(self.address, x, y, r1, r2, text, color)
end

function Display:initialize()
    return cl_display_initialize(self.address)
end
function Display:new(address)
    local o = setmetatable(TinyBlockBase:new(address), Display)
    print("display address", o.address)
    o.ssd_address = 0
    o:initialize()
    return o
end
setmetatable(Display, {__index = TinyBlockBase})
return Display
