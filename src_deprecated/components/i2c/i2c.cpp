/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#   include <Arduino.h>
#   include <Wire.h>

#include <stdint.h>
#include <esp_log.h>
#include <esp_err.h>
#include <hugo_tiny_block_defines.h>

#define  I2C_NUM I2C_NUM_0
static const char *TAG = "I2C";

#define I2C_RATE 100000
//static uint8_t buffer[129];

#ifdef __cplusplus
extern "C" {
#endif

static _lock_t s_lock;

bool hugo_i2c_tiny(
    uint8_t address, uint8_t block_type_id, uint8_t command, uint8_t *out_data, uint8_t out_data_size, uint8_t *in_data, uint8_t in_data_size
){
    //ESP_LOGI(TAG, "start for address: %d", address);
    _lock_acquire(&s_lock);

    //for (int i = 60; i < 62; i++){
    //    address = i;
        Wire.beginTransmission(address);
        if (block_type_id != I2C_BLOCK_TYPE_ID_NONE){
            Wire.write(block_type_id); // write the register address
        }

        if (command != I2C_COMMAND_NONE){
            Wire.write(command); // write the register address
        }
        if (out_data_size > 0){
            if (Wire.write(out_data, out_data_size) != out_data_size){ // write the data
                ESP_LOGE(TAG, "data not written to address: %d", address);
            }
        }
        uint8_t error = Wire.endTransmission(true);
        if (error != 0){
            ESP_LOGE(TAG, "transmission error %d for address: %d", error, address);
        }
        else {
            //ESP_LOGI(TAG, "transmission OK for address: %d", address);
        }
    //}

    bool result = true;
    if (in_data_size != 0){
        Wire.requestFrom(address, in_data_size);
        for (int index = 0; index < in_data_size; ++index){
            if (!Wire.available()){
                ESP_LOGE(TAG, "I2C data no longer available for address: %d", address);
                result = false;
                break;
            }
            in_data[index] = Wire.read();
        }
    }
    _lock_release(&s_lock);
    //ESP_LOGI(TAG, "OK for address: %d", address);
    return result;
}

bool hugo_i2c(uint8_t address, uint8_t command, uint8_t *out_data, unsigned out_data_size, uint8_t *in_data, unsigned in_data_size){
    return hugo_i2c_tiny(address, I2C_BLOCK_TYPE_ID_NONE, command, out_data, out_data_size, in_data, in_data_size);
}

bool hugo_i2c_init(int sda_pin, int scl_pin)
{
    _lock_acquire(&s_lock);
    Wire.begin(sda_pin, scl_pin);
    Wire.setClock(I2C_RATE);
    _lock_release(&s_lock);
    return true;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */