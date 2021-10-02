#pragma once

#include <lua.h>
#include <stdbool.h>

typedef enum {
    MPU9250_FILL_NONE = 0,
    MPU9250_FILL_TEMPERATURE = 1 << 0,
    MPU9250_FILL_ACCEL_X = 1 << 1,
    MPU9250_FILL_ACCEL_Y = 1 << 2,
    MPU9250_FILL_ACCEL_Z = 1 << 3,
    MPU9250_FILL_GYRO_X = 1 << 4,
    MPU9250_FILL_GYRO_Y = 1 << 5,
    MPU9250_FILL_GYRO_Z = 1 << 6,
    MPU9250_FILL_MAG_X = 1 << 7,
    MPU9250_FILL_MAG_Y = 1 << 8,
    MPU9250_FILL_MAG_Z = 1 << 9
} mpu9250_fill;

bool hugo_mpu9250_init(lua_State* L, mpu9250_fill fill_mask);
