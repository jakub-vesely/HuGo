--[[
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]
TinyBlockBase = require "tiny_block_base"
local IrReceiver = {}
IrReceiver.__index = IrReceiver

function IrReceiver:get_ir_data()
    return cl_get_ir_data(self.address)
end

function IrReceiver:new(address)
    local o = setmetatable(TinyBlockBase:new(address), IrReceiver)
    print("ir_receiver address", o.address)
    o.ina_address = 0
    return o
end
setmetatable(IrReceiver, {__index = TinyBlockBase})

return IrReceiver