--[[
Copyright (c) 2021 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]

local MotorBlock = {}
MotorBlock.address = nil
function MotorBlock:new(address)
    assert(type(address) == "number", "tiny-block address is not set as a number")
    o = {}
    o.address = address
    setmetatable(o, self)
    self.__index = self
    return o
end

function MotorBlock:turn_clockwise(motorId)
    cl_motor_turn_clockwise(self.address, motorId)
end

function MotorBlock:turn_anticlockwise(motorId)
    cl_motor_turn_anticlockwise(self.address, motorId)
end

function MotorBlock:power_on(motorId)
    cl_motor_power_on(self.address, motorId)
end

function MotorBlock:power_off(motorId)
    cl_motor_power_off(self.address, motorId)
end

function MotorBlock:stop(motorId)
    cl_motor_stop(self.address, motorId)
end

function MotorBlock:reset_counter(motorId)
    cl_motor_reset_counter(self.address, motorId)
end

function MotorBlock:get_counter(motorId)
    return cl_motor_get_counter(self.address, motorId)
end

function MotorBlock:speed_100(motorId)
    cl_motor_speed_100(self.address, motorId)
end

function MotorBlock:speed_80(motorId)
    cl_motor_speed_80(self.address, motorId)
end

function MotorBlock:speed_60(motorId)
    cl_motor_speed_60(self.address, motorId)
end

function MotorBlock:speed_40(motorId)
    cl_motor_speed_40(self.address, motorId)
end

function MotorBlock:speed_20(motorId)
    cl_motor_speed_20(self.address, motorId)
end

function MotorBlock:speed_0(motorId)
    cl_motor_speed_0(self.address, motorId)
end

return MotorBlock