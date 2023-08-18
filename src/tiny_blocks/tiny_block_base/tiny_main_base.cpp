#include <stdint.h>
#include <Wire.h>
#include <tiny_main_base.h>
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
  if (s_common_buffer.size != 0){
    Wire.write(s_common_buffer.data, s_common_buffer.size);
  }
  uint8_t error = Wire.endTransmission();     // finish transmission

  if (error != 0){
    return false;
  }

  if (expected_response_size != 0){
    uint8_t read_size = Wire.requestFrom(address, expected_response_size);
    s_common_buffer.size = 0;
    if (expected_response_size != 0xff && read_size != expected_response_size){
      return false;
    }
    for (uint8_t index = 0; index < read_size; index++){
      uint8_t byte = Wire.read();
      //first byte is not important in this implementation because we have number of read bytes
      if (expected_response_size != 0xff || index != 0){
          s_common_buffer.data[s_common_buffer.size++] = byte;
      }
    }
  }
  return true;
}

bool tiny_main_base_send_i2c_message(uint8_t address, uint8_t expected_response_size){
  return tiny_main_base_send_i2c_command(address, 0x00, 0x00, expected_response_size);
}

uint8_t tiny_main_base_get_ext_module_address(uint8_t address){
  tiny_main_base_send_i2c_command(address, I2C_BLOCK_TYPE_ID_BASE, GET_EXT_ADDRESS_COMMAND, 1);
  if (s_common_buffer.size != 1){
    return 0;
  }
  return  s_common_buffer.data[0];
}
