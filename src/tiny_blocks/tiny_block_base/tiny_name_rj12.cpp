#include "tiny_main_rj12.h"
#include <hugo_defines.h>

static tiny_common_buffer_t* p_common_buffer = tiny_main_base_get_common_buffer();

void tiny_main_rj12_set_pin_mode(uint8_t block_address, Rj12PinId pin, Rj12PinMode mode){
    uint8_t command;
    if (pin == 4){
        command = mode == input ? I2C_COMMAND_RJ12_SET_PIN4_AS_IN : I2C_COMMAND_RJ12_SET_PIN4_AS_OUT;
    }
    else{ //5
        command = mode == input ? I2C_COMMAND_RJ12_SET_PIN5_AS_IN : I2C_COMMAND_RJ12_SET_PIN5_AS_OUT;
    }

    tiny_main_base_send_i2c_command(block_address, I2C_BLOCK_TYPE_ID_RJ12, command, 0);
}

void tiny_main_rj12_set_pin_value(uint8_t block_address, Rj12PinId pin, bool value){
    p_common_buffer->data[0] = value ? 1 : 0;
    p_common_buffer->size = 1;

    tiny_main_base_send_i2c_command(
        block_address,
        I2C_BLOCK_TYPE_ID_RJ12,
        pin == 4 ? I2C_COMMAND_RJ12_SET_PIN4_DIG_VAL : I2C_COMMAND_RJ12_SET_PIN5_DIG_VAL,
        0);
}

bool tiny_main_rj12_get_pin_value_digital(uint8_t block_address, Rj12PinId pin){
    bool ret_val = tiny_main_base_send_i2c_command(
        block_address,
        I2C_BLOCK_TYPE_ID_RJ12,
        pin == pin4 ? I2C_COMMAND_RJ12_GET_PIN4_DIG_VALUE : I2C_COMMAND_RJ12_GET_PIN5_DIG_VALUE,
        1
    );
    if (ret_val && p_common_buffer->size == 1){
        return p_common_buffer->data[0] > 0;
  }
  return false;
}

uint16_t tiny_main_rj12_get_pin_value_analog(uint8_t block_address, Rj12PinId pin){
    bool ret_val = tiny_main_base_send_i2c_command(
        block_address,
        I2C_BLOCK_TYPE_ID_RJ12,
        pin == pin4 ? I2C_COMMAND_RJ12_GET_PIN4_ANA_VALUE : I2C_COMMAND_RJ12_GET_PIN5_ANA_VALUE,
        2
    );
    if (ret_val && p_common_buffer->size == 2){
        return (p_common_buffer->data[0] << 8) + p_common_buffer->data[1];
    }
  return -1;
}
