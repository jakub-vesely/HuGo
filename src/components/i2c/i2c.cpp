/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */
#define ARDUINO_I2c

#ifdef ARDUINO_I2c
#   include <Arduino.h>
#   include <Wire.h>
#else
#   include <driver/i2c.h>
#endif

#include <stdint.h>
#include <esp_log.h>
#include <esp_err.h>

#define  I2C_NUM I2C_NUM_0
#define TAG "i2c"

#define I2C_RATE 100000
static uint8_t buffer[129];

#ifdef __cplusplus
extern "C" {
#endif

static _lock_t s_lock;

static bool _write_command_with_data(int address, int command, uint8_t *data, unsigned data_size)
{
#ifdef ARDUINO_I2c
    _lock_acquire(&s_lock);

    Wire.beginTransmission(address);
    if (command >= 0)
    {
        Wire.write(command); // write the register address
    }
    Wire.write(data, data_size); // write the data

    Wire.endTransmission(true);

    // int index = 0;
    // if (command >= 0)
    // {
    //     buffer[index] = command;
    //     index++;
    // }
    // memcpy(buffer + index, data, data_size);
    // Wire.writeTransmission(address, buffer, data_size + index, true);

    _lock_release(&s_lock);
    return true;
#else
    _lock_acquire(&s_lock);

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);

    if (command >= 0) //it is expected that command is uint8_t and a valid command is not negative
    {
        i2c_master_write_byte(cmd, command, true);
    }

    i2c_master_write(cmd, data, data_size, true);

    i2c_master_stop(cmd);
    esp_err_t espRc = i2c_master_cmd_begin(I2C_NUM, cmd, 10/portTICK_PERIOD_MS);
    if (espRc != ESP_OK)
    {
        ESP_LOGE(TAG, "I2C write failed with code: 0x%.2X", espRc);
    }
    i2c_cmd_link_delete(cmd);
    _lock_release(&s_lock);
    return espRc == ESP_OK;
#endif
}

bool hugo_i2c_read_data(int address, uint8_t * data, size_t size){
    _lock_acquire(&s_lock);

    Wire.requestFrom(address, size);
    for (int index = 0; index < size; ++index){
        if (!Wire.available()){
            ESP_LOGE(TAG, "I2C data no longer available");
            return false;
        }
        data[index] = Wire.read();
    }
    _lock_release(&s_lock);
    return true;
}

bool hugo_i2c_write_command_with_data(int address, uint8_t command, uint8_t *data, unsigned data_size)
{
    return _write_command_with_data(address, command, data, data_size);
}

bool hugo_i2c_write_data(int address, uint8_t * data, unsigned size)
{
    return _write_command_with_data(address, -1, data, size);
}

bool hugo_i2c_init(int sda_pin, int scl_pin)
{
#ifdef ARDUINO_I2c
    _lock_acquire(&s_lock);
    Wire.begin(sda_pin, scl_pin);
    Wire.setClock(I2C_RATE);
    _lock_release(&s_lock);
    return true;
#else
    _lock_acquire(&s_lock);
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_pin,
        .scl_io_num = scl_pin,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 1000000
    };
    bool ret_val = false;
    if (i2c_param_config(I2C_NUM, &i2c_config) != ESP_OK){
        ret_val = false;
    }
    else{
        ret_val = i2c_driver_install(I2C_NUM, I2C_MODE_MASTER, 0, 0, 0) == ESP_OK;
    }
    _lock_release(&s_lock);
    return ret_val;
#endif
}

#ifdef __cplusplus
}
#endif /* __cplusplus */