#include "tiny_main_power.h"
#include <string.h>
#include <hugo_defines.h>
#include <tiny_main_base.h>

static uint8_t s_power_ina_address = 0;
static tiny_common_buffer_t* p_common_buffer = tiny_main_base_get_common_buffer();

#define I2C_COMMAND_POWER_GET_STATE 0x01

#define _INA219_MODE_SHUNT_BUS_CONTINUOUS   0b111
#define _INA219_SADC_8SAM_4260              0b1011
#define _INA219_BADC_RES_12BIT              0b0011
#define _INA219_PG_GD4_160MV                0b10
#define _INA219_BRNG_16V                    0b00

#define _INA219_CONFIGURATION_COMMAND       0x00
#define _INA219_SHUNT_VOLTAGE_COMMAND       0x01
#define _INA219_BUSVOLTAG_COMMAND           0x02


void tiny_main_power_init(bool on){
  s_power_ina_address = tiny_main_base_get_ext_module_address(I2C_BLOCK_TYPE_ID_POWER);
  tiny_main_power_power_on(on);
}

bool tiny_main_power_is_available(){
  return s_power_ina_address != 0;
}

void tiny_main_power_power_on(bool on){
  if (!tiny_main_power_is_available()){
    return;
  }

  uint16_t config = 0;// | (_INA219_SADC_8SAM_4260 << 3) | (_INA219_BADC_RES_12BIT << 7) | (_INA219_PG_GD4_160MV << 11) | (_INA219_BRNG_16V << 13);
  if (on){
    config =  _INA219_MODE_SHUNT_BUS_CONTINUOUS | (_INA219_SADC_8SAM_4260 << 3) | (_INA219_BADC_RES_12BIT << 7) | (_INA219_PG_GD4_160MV << 11) | (_INA219_BRNG_16V << 13);
  }
  p_common_buffer->size = 3;
  p_common_buffer->data[0] = _INA219_CONFIGURATION_COMMAND;
  p_common_buffer->data[1] = config & 0xff;
  p_common_buffer->data[2] = config >> 8;
  tiny_main_base_send_i2c_message(s_power_ina_address, 0);
}

charging_state_t tiny_main_power_get_charging_state(){

  p_common_buffer->size = 0;
  uint8_t ret_val = tiny_main_base_send_i2c_command(I2C_BLOCK_TYPE_ID_POWER, I2C_BLOCK_TYPE_ID_POWER, I2C_COMMAND_POWER_GET_STATE, 1);
  charging_state_t state = {false, false};

  if (ret_val && p_common_buffer->size == 1){
    state.is_usb_connected = (p_common_buffer->data[0] >> 1) & 1;
    state.is_charging = (p_common_buffer->data[0] & 1) == 1 ? 0 : 1;
  }
  return state;
}

int32_t tiny_main_power_get_bat_voltage_mV(){
  if (!tiny_main_power_is_available()){
    return 0;
  }

  p_common_buffer->size = 1;
  p_common_buffer->data[0] = _INA219_BUSVOLTAG_COMMAND;
  tiny_main_base_send_i2c_message(s_power_ina_address, 2);

  return (((p_common_buffer->data[0] << 8) | p_common_buffer->data[1]) >> 3) * 4;
}

int32_t tiny_main_power_get_bat_current_uA(){
  if (!tiny_main_power_is_available()){
    return 0;
  }

  p_common_buffer->size = 1;
  p_common_buffer->data[0] = _INA219_SHUNT_VOLTAGE_COMMAND;
  tiny_main_base_send_i2c_message(s_power_ina_address, 2);

  if (p_common_buffer->size != 2){
    return 0;
  }
  uint16_t raw_voltage =  (((uint16_t)p_common_buffer->data[0]) << 8) | p_common_buffer->data[1];
  bool negative = true; //charging will be shown as negative
  int32_t current_uA = 0;
  if (p_common_buffer->data[0] & 0x80){
    raw_voltage = ((raw_voltage - 1) ^ 0xffff);
    negative = false;
    current_uA = raw_voltage * 100; //here is direct multiplication required
  }
  else{
    current_uA = raw_voltage; //direct multiplication leads to wrong result
    current_uA *= 100;
  }

  //raw_voltage * 100; //(1000 * (0.01 / _SHUNT_R)); _SHUNT_R=0.1

  //current_uA *= 100;
  if (negative){
    current_uA = -current_uA;
  }
  return current_uA;
}
