--[[
Copyright (c) 2021 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]

TinyBlockBase = require "tiny_block_base"
local MotorDriver = {}
MotorDriver.__index = MotorDriver
MotorDriver.address = nil

function MotorDriver:turn_clockwise(motorId)
    cl_motor_turn_clockwise(self.address, motorId)
end

function MotorDriver:turn_anticlockwise(motorId)
    cl_motor_turn_anticlockwise(self.address, motorId)
end

function MotorDriver:power_on(motorId)
    cl_motor_power_on(self.address, motorId)
end

function MotorDriver:power_off(motorId)
    cl_motor_power_off(self.address, motorId)
end

function MotorDriver:stop(motorId)
    cl_motor_stop(self.address, motorId)
end

function MotorDriver:reset_counter(motorId)
    cl_motor_reset_counter(self.address, motorId)
end

function MotorDriver:get_counter(motorId)
    return cl_motor_get_counter(self.address, motorId)
end

function MotorDriver:speed_100(motorId)
    cl_motor_speed_100(self.address, motorId)
end

function MotorDriver:speed_80(motorId)
    cl_motor_speed_80(self.address, motorId)
end

function MotorDriver:speed_60(motorId)
    cl_motor_speed_60(self.address, motorId)
end

function MotorDriver:speed_40(motorId)
    cl_motor_speed_40(self.address, motorId)
end

function MotorDriver:speed_20(motorId)
    cl_motor_speed_20(self.address, motorId)
end

function MotorDriver:speed_0(motorId)
    cl_motor_speed_0(self.address, motorId)
end

function MotorDriver:new(address)
    local o = setmetatable(TinyBlockBase:new(address), MotorDriver)
    print("motor_driver address", o.address)
    return o
end
setmetatable(MotorDriver, {__index = TinyBlockBase})

return MotorDriver