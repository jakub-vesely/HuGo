#include <stdint.h>
#include <Wire.h>
#include "tiny_main_base.h"
#include <hugo_defines.h>

#if defined(__AVR_ATtiny414__) || defined(__AVR_ATtiny1614__)
# define LED_PIN PIN_PA4
#endif

static tiny_common_buffer_t s_common_buffer;
tiny_common_buffer_t* tiny_main_base_get_common_buffer(){
  return &s_common_buffer;
}

void tiny_main_base_init(){
#if defined(__AVR_ATtiny414__) || defined(__AVR_ATtiny1614__)
  pinMode(LED_PIN, OUTPUT);
  PORTMUX.CTRLB |= PORTMUX_TWI0_ALTERNATE_gc; //_ATtiny414 uses alternative ports for i2c because of conflict with the POWER_SAVE_PIN
#endif
  Wire.begin();
  delay(100);
}

#if defined(__AVR_ATtiny414__) || defined(__AVR_ATtiny1614__)
void tiny_main_base_set_build_in_led(bool state){
  digitalWrite(LED_PIN, state ? HIGH: LOW);
}
#endif

/**
when expected_response_size == 0, no resoponse will be requested, when requested_size == 0xff fist byte contains size of data
*/
bool tiny_main_base_send_i2c_command(uint8_t address, uint8_t block_type_id, uint8_t command, uint8_t expected_response_size){
  Wire.beginTransmission(address);
  if (command != 0){
    Wire.write(block_type_id);
    Wire.write(command);
  }

  for (uint8_t index = 0; index < s_common_buffer.size; index++){
    //workaround: it seems size of I2C buffer is 16 bytes and this buffer is used when writing bytes one by one
    //it seems the problem could be on the receiver side which has 16B buffer only because RAM is < 256
    //TODO: should be 32 for attiny412 as well in base of the documentation
    if ((index + 1) % 16 == 0){
      if (Wire.endTransmission(true) != 0){
        return false;
      }
      Wire.beginTransmission(address);
      if (command != 0){
        Wire.write(block_type_id);
        Wire.write(command);
      }
    }
    Wire.write(s_common_buffer.data[index]);
  }

  if (Wire.endTransmission(true) != 0){
    return false;
  }

  s_common_buffer.size = 0; //for sure also for case no response size is set
  if (expected_response_size != 0){
    uint8_t read_size = Wire.requestFrom(address, expected_response_size);

    if (expected_response_size != 0xff && read_size != expected_response_size){
      return false;
    }

    uint8_t count = read_size;
    if (expected_response_size == 0xff && read_size > 0){
      count = Wire.read();
      read_size--;
    }

    for (uint8_t index = 0; index < count; index++){
      s_common_buffer.data[s_common_buffer.size++] = Wire.read();
    }
  }
  delay(200);
  return true;
}

bool tiny_main_base_send_i2c_message(uint8_t address, uint8_t expected_response_size){
  return tiny_main_base_send_i2c_command(address, 0x00, 0x00, expected_response_size);
}

uint8_t tiny_main_base_get_ext_module_address(uint8_t address){
  tiny_main_base_send_i2c_command(address, I2C_BLOCK_TYPE_ID_BASE, I2C_COMMAND_GET_EXT_ADDRESS, 1);
  if (s_common_buffer.size != 1){
    return 0;
  }
  return  s_common_buffer.data[0];
}

void tiny_main_base_set_power_save(uint8_t address, uint8_t mode){
  s_common_buffer.data[0] = mode;
  s_common_buffer.size = 1;
  tiny_main_base_send_i2c_command(address, I2C_BLOCK_TYPE_ID_BASE, I2C_COMMAND_SET_POWER_SAVE, 0);

}
