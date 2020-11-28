#pragma once

int mpu9250_begin();
void mpu9250_read_sensor();
float mpu9250_get_temperature();

float mpu9250_get_accel_x();
float mpu9250_get_accel_y();
float mpu9250_get_accel_z();

float mpu9250_get_gyro_x();
float mpu9250_get_gyro_y();
float mpu9250_get_gyro_z();

float mpu9250_get_mag_x();
float mpu9250_get_mag_y();
float mpu9250_get_mag_z();
