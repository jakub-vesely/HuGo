--[[
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]

TinyBlockBase = require "tiny_block_base"

local Rgb = {}
Rgb.__index = Rgb

Rgb.color = {
black = 1,
white = 2,
red = 3,
green = 4,
blue = 5,
yellow = 6,
purple = 7,
cyan =8,
orange = 9,
greenyellow = 10,
skyblue = 11,
aquamarine = 12,
magenta = 13,
violet = 14
}

function Rgb:set_values(red, green, blue)
    cl_rgb_set_values(self.address, red, green, blue)
end

function Rgb:set_on()
    cl_rgb_set_on(self.address)
end

function Rgb:set_off()
    cl_rgb_set_off(self.address)
end

function Rgb:set_color(color)
    cl_rgb_set_color(self.address, color)
end

function Rgb:new(address)
    local o = setmetatable(TinyBlockBase:new(address), Rgb)
    print("rgb address", o.address)
    return o
end
setmetatable(Rgb, {__index = TinyBlockBase})

return Rgb