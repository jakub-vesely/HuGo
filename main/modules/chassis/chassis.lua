--[[
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]

local chassis =  {}

function chassis.stop()
    return cl_chassis_stop()
end

function chassis.go_forward()
    return cl_chassis_go_forward()
end

function chassis.go_backward()
    return cl_chassis_go_backward()
end

function chassis.rotate_clockwise()
    return cl_chassis_rotate_clockwise()
end

function chassis.rotate_counterclockwise()
    return cl_chassis_rotate_counterclockwise()
end

return chassis
