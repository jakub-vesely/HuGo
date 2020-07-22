/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#include <driver/i2c.h>
#include <stdint.h>
#include <esp_log.h>
#include <esp_err.h>

#define  I2C_NUM I2C_NUM_0
#define TAG "i2c_wrapper"

static bool _i2c_master_init(int sda_pin, int scl_pin)
{
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_pin,
        .scl_io_num = scl_pin,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 1000000
    };
    if (i2c_param_config(I2C_NUM, &i2c_config) != ESP_OK)
        return false;
    return i2c_driver_install(I2C_NUM, I2C_MODE_MASTER, 0, 0, 0) == ESP_OK;
}

static bool _write_command_with_data(int address, int command, uint8_t *data, size_t data_size)
{
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
    return espRc == ESP_OK;
}


bool hugo_i2c_write_command_with_data(int address, uint8_t command, uint8_t *data, size_t data_size)
{
    return _write_command_with_data(address, command, data, data_size);
}

bool hugo_i2c_write_data(int address, uint8_t * data, size_t size)
{
    return _write_command_with_data(address, -1, data, size);
}

bool hugo_i2c_init(int sda_pin, int scl_pin)
{
    return _i2c_master_init(sda_pin, scl_pin);
}
