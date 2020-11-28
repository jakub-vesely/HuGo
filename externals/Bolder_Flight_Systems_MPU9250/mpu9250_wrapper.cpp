g
#include <MPU9250_fork/src/MPU9250.h>
#include <Wire.h>

#ifdef __cplusplus
extern "C" {
#endif

static MPU9250 s_mpu9250(Wire, 0x68);

int mpu9250_begin()
{
    return s_mpu9250.begin();
}

void mpu9250_read_sensor()
{
    s_mpu9250.readSensor();
}

float mpu9250_get_temperature()
{
    return s_mpu9250.getTemperature_C();
}

float mpu9250_get_accel_x()
{
    return s_mpu9250.getAccelX_mss();
}

float mpu9250_get_accel_y()
{
    return s_mpu9250.getAccelY_mss();
}

float mpu9250_get_accel_z()
{
    return s_mpu9250.getAccelZ_mss();
}

float mpu9250_get_gyro_x()
{
    return s_mpu9250.getGyroX_rads();
}

float mpu9250_get_gyro_y()
{
    return s_mpu9250.getGyroY_rads();
}

float mpu9250_get_gyro_z()
{
    return s_mpu9250.getGyroZ_rads();
}

float mpu9250_get_mag_x()
{
    return s_mpu9250.getMagX_uT();
}

float mpu9250_get_mag_y()
{
    return s_mpu9250.getMagY_uT();
}

float mpu9250_get_mag_z()
{
    return s_mpu9250.getMagZ_uT();
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
