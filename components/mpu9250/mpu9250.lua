--[[
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]

local mpu9250 = {
    measured_actions = {}
}

function mpu9250.get_temperature()
    return cl_mpu9250_get_temperature();
end

function mpu9250.get_accel_x()
    return cl_mpu9250_get_accel_x();
end

function mpu9250.get_accel_y()
    return cl_mpu9250_get_accel_y();
end

function mpu9250.get_accel_z()
    return cl_mpu9250_get_accel_z();
end

function mpu9250.get_gyro_x()
    return cl_mpu9250_get_gyro_x();
end

function mpu9250.get_gyro_y()
    return cl_mpu9250_get_gyro_y();
end

function mpu9250.get_gyro_z()
    return cl_mpu9250_get_gyro_z();
end

function mpu9250.get_mag_x()
    return cl_mpu9250_get_mag_x();
end

function mpu9250.get_mag_y()
    return cl_mpu9250_get_mag_y();
end

function mpu9250.get_mag_z()
    return cl_mpu9250_get_mag_z();
end

function mpu9250_measured_callback(addr, code, repeated)
    for index, action in pairs(mpu9250.measured_actions) do
        action(addr, code, repeated)
    end
end

function mpu9250.add_measured_action(action_function)
    table.insert(mpu9250.measured_actions, action_function)
end


return mpu9250
