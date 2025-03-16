#include "tiny_main_rj12.h"
#include <hugo_defines.h>
#include <tiny_main_base.h>
#include <string.h>

static tiny_common_buffer_t* p_common_buffer = tiny_main_base_get_common_buffer();

void tiny_main_rj12_set_pin_mode(uint8_t block_address, Rj12PinId pin, Rj12PinMode mode){
    uint8_t command = I2C_COMMAND_RJ12_NO_COMMAND;
    switch (mode){
        case Rj12PinMode::input:
            command = pin == Rj12PinId::pin4 ? I2C_COMMAND_RJ12_PIN4_SET_AS_IN : I2C_COMMAND_RJ12_PIN5_SET_AS_IN;
        break;
        case Rj12PinMode::output:
            command = pin == Rj12PinId::pin4 ? I2C_COMMAND_RJ12_PIN4_SET_AS_OUT : I2C_COMMAND_RJ12_PIN5_SET_AS_OUT;
        break;
        case Rj12PinMode::interrupt_rising:
            if (pin == Rj12PinId::pin4){
                command = I2C_COMMAND_RJ12_PIN4_SET_AS_COUNTER_RISI;
            }
        break;
        case Rj12PinMode::interrupt_falling:
            if (pin == Rj12PinId::pin4){
                command = I2C_COMMAND_RJ12_PIN4_SET_AS_COUNTER_FALL;
            }
        break;
        case Rj12PinMode::interrupt_change:
            if (pin == Rj12PinId::pin4){
                command = I2C_COMMAND_RJ12_PIN4_SET_AS_COUNTER_CHNG;
            }
        break;
    }
    p_common_buffer->size = 0;
    tiny_main_base_send_i2c_command(block_address, I2C_BLOCK_TYPE_ID_RJ12, command, 0);
}

void tiny_main_rj12_set_pin_value(uint8_t block_address, Rj12PinId pin, bool value){
    p_common_buffer->data[0] = value ? 1 : 0;
    p_common_buffer->size = 1;

    tiny_main_base_send_i2c_command(
        block_address,
        I2C_BLOCK_TYPE_ID_RJ12,
        pin == 4 ? I2C_COMMAND_RJ12_PIN4_SET_DIG_VALUE : I2C_COMMAND_RJ12_PIN5_SET_DIG_VALUE,
        0);
}

bool tiny_main_rj12_get_pin_value_digital(uint8_t block_address, Rj12PinId pin){
    p_common_buffer->size = 0;
    bool ret_val = tiny_main_base_send_i2c_command(
        block_address,
        I2C_BLOCK_TYPE_ID_RJ12,
        pin == pin4 ? I2C_COMMAND_RJ12_PIN4_GET_DIG_VALUE : I2C_COMMAND_RJ12_PIN5_GET_DIG_VALUE,
        1
    );
    if (ret_val && p_common_buffer->size == 1){
        return p_common_buffer->data[0] > 0;
    }
    return false;
}

uint16_t tiny_main_rj12_get_pin_value_analog(uint8_t block_address, Rj12PinId pin){
    p_common_buffer->size = 0;
    bool ret_val = tiny_main_base_send_i2c_command(
        block_address,
        I2C_BLOCK_TYPE_ID_RJ12,
        pin == pin4 ? I2C_COMMAND_RJ12_PIN4_GET_ANA_VALUE : I2C_COMMAND_RJ12_PIN5_GET_ANA_VALUE,
        2
    );
    if (ret_val && p_common_buffer->size == 2){
        return (p_common_buffer->data[1] << 8) + p_common_buffer->data[0];
    }
    return -1;
}

uint16_t tiny_main_rj12_pin4_get_count_and_reset(uint8_t block_address){
    p_common_buffer->size = 0;
    bool ret_val = tiny_main_base_send_i2c_command(
        block_address,
        I2C_BLOCK_TYPE_ID_RJ12,
        I2C_COMMAND_RJ12_PIN4_GET_COUNT_AND_RESET,
        2
    );

    if (not ret_val || p_common_buffer->size != 2){
        return 0;
    }

    return (p_common_buffer->data[1] << 8) + p_common_buffer->data[0];
}

bool tiny_main_rj12_pin4_get_timestamp_diffs(uint8_t block_address){
    p_common_buffer->size = 0;
    bool ret_val = tiny_main_base_send_i2c_command(
        block_address,
        I2C_BLOCK_TYPE_ID_RJ12,
        I2C_COMMAND_RJ12_PIN4_GET_LAST_TIMESTAMPS,
        8
    );

    if (not ret_val || p_common_buffer->size != 8){
        return false;
    }

    //result is returned in p_common_buffer
    return true;
}

bool tiny_main_rj12_pin5_set_oscil_period_ms(uint8_t block_address, uint16_t period_ms){
    p_common_buffer->data[0] = period_ms & 0xff;
    p_common_buffer->data[1] = period_ms >> 8;
    p_common_buffer->size = 2;

    return tiny_main_base_send_i2c_command(
        block_address,
        I2C_BLOCK_TYPE_ID_RJ12,
        I2C_COMMAND_RJ12_PIN4_SET_OSCIL_PERIOD_MS,
        0
    );
}
