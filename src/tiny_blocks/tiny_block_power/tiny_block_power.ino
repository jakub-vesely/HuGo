/*
power board management

BOARD: hugo_adapter + hugo_power
Chip: ATtiny412, 414, 1614
Clock Speed: 1MHz
Programmer: jtag2updi (megaTinyCore)
*/

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);
uint8_t HugoTinyWireGetExtAddress();
void HugoTinyWireChangeExtAddress(uint8_t address);
void HugoTinyWireFillModuleVersion();

#define ADJUSTMENT_VERSION 0

#define HUGO_TINY_ONE_EXTENSION

#include <hugo_tiny_wire.h>
#include <hugo_gpio.h>

#define EEPROM_A0A1_POS EEPROM_RESERVED_MAX + 1
#define INA_I2C_ADDRESS_NA0_NA1 0x40
#define INA_I2C_ADDRESS_A0_NA1  0x41
#define INA_I2C_ADDRESS_NA0_A1  0x44
#define INA_I2C_ADDRESS_A0_A1   0x45 //should be the default one

#define INA_A0_PIN        HUGO_PIN_D1
#define INA_A1_PIN        HUGO_PIN_D2
#define CHARGE_STATE_PIN  HUGO_PIN_D3

#if !defined(__AVR_ATtiny412__) && HUGO_PCB_VERSION >= 7
# define POWER_ENABLE_PIN  HUGO_PIN_D5
#endif

#define CHARGING_TRESHOLD (int)(1024.0 / 3.3) //1V
#define USB_TRESHOLD (int)(1024.0 / 3.3 * 2.5) //2V

#define I2C_COMMAND_POWER_GET_STATE      0x01
#define I2C_COMMAND_POWER_POWER_OFF      0x02

static const uint8_t ext1_addresses[5] = {
  INA_I2C_ADDRESS_NA0_NA1, INA_I2C_ADDRESS_A0_NA1, INA_I2C_ADDRESS_NA0_A1, INA_I2C_ADDRESS_A0_A1, 0x00
};
static const uint8_t* ext_addresses[2] = { (uint8_t*)ext1_addresses, NULL };
static uint8_t s_current_ext_address = 0;

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
  switch (command){

    case I2C_COMMAND_POWER_GET_STATE:
      uint16_t charging_pin_value = analogRead(CHARGE_STATE_PIN); //10 bit A/D converter
      bool is_charging = charging_pin_value > CHARGING_TRESHOLD && charging_pin_value < USB_TRESHOLD;
      bool is_usb = charging_pin_value > USB_TRESHOLD || is_charging; //USB is prerequisite for charging
      s_buffer.data[0] = (is_usb ? 2 : 0) | (is_charging ? 1 : 0);
      s_buffer.size = 1;
      tiny_main_base_set_build_in_led_a(is_charging);
      tiny_main_base_set_build_in_led_b(is_usb);
      break;
    case I2C_COMMAND_POWER_POWER_OFF:
#if !defined(__AVR_ATtiny412__) && HUGO_PCB_VERSION >= 7
      digitalWrite(POWER_ENABLE_PIN, false);
#endif
      break;
  }
}

uint8_t HugoTinyWireGetExtAddress(){
  return s_current_ext_address;
}

void HugoTinyWireChangeExtAddress( uint8_t address){
  s_current_ext_address = address;
  uint8_t a0_value = (address == INA_I2C_ADDRESS_A0_NA1 || address == INA_I2C_ADDRESS_A0_A1 ? 1 : 0);
  uint8_t a1_value = (address == INA_I2C_ADDRESS_NA0_A1 || address == INA_I2C_ADDRESS_A0_A1 ? 1 : 0);
  EEPROM.write(EEPROM_A0A1_POS, (a1_value << 1) | a0_value);
  digitalWrite(INA_A0_PIN, a0_value);
  digitalWrite(INA_A1_PIN, a1_value);
}

void HugoTinyWireFillModuleVersion(){
  s_buffer.data[0] = I2C_BLOCK_TYPE_ID_POWER;
  s_buffer.data[1] = HUGO_PCB_VERSION;
  s_buffer.data[2] = ADJUSTMENT_VERSION;
  s_buffer.size = 3;
}

void HugoTinyWirePowerSave(uint8_t level){
}

void setup(){
  uint8_t a_values =  EEPROM.read(EEPROM_A0A1_POS); //default value should be 0xFF
  uint8_t a0_value =  a_values & 1;
  uint8_t a1_value =  (a_values >> 1) & 1;
  if (a0_value == 0 && a1_value == 0){
    s_current_ext_address = INA_I2C_ADDRESS_NA0_NA1;
  }
  else if (a0_value != 0 && a1_value == 0){
    s_current_ext_address = INA_I2C_ADDRESS_A0_NA1;
  }
  else if (a0_value == 0 && a1_value != 0){
    s_current_ext_address = INA_I2C_ADDRESS_NA0_A1;
  }
  else {
    s_current_ext_address = INA_I2C_ADDRESS_A0_A1;
  }

  HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_POWER, (uint8_t**)ext_addresses);

#if !defined(__AVR_ATtiny412__) && HUGO_PCB_VERSION >=7
  pinMode(POWER_ENABLE_PIN, OUTPUT);
  digitalWrite(POWER_ENABLE_PIN, true);
#endif

  pinMode(INA_A0_PIN, OUTPUT);
  pinMode(INA_A1_PIN, OUTPUT);
  digitalWrite(INA_A0_PIN, a0_value);
  digitalWrite(INA_A1_PIN, a1_value);

  pinMode(HUGO_PIN_LED_A, OUTPUT);
  pinMode(HUGO_PIN_LED_B, OUTPUT);
  digitalWrite(HUGO_PIN_LED_A, false);
  digitalWrite(HUGO_PIN_LED_B, false);

  pinMode(CHARGE_STATE_PIN, INPUT); //there is a pull down resistor on the power shield
}

void loop(){
  // for (int index=0; index < 200; index++){
  //   delay(500);
  //   digitalWrite(CHARGE_STATE_PIN, true);
  //   delay(500);
  //   digitalWrite(CHARGE_STATE_PIN, false);
  // }

  // for (int index=0; index < 10; index++){
  //   delay(500);
  //   digitalWrite(HUGO_PIN_LED_B, true);
  //   delay(500);
  //   digitalWrite(HUGO_PIN_LED_B, false);
  // }
  // digitalWrite(HUGO_PIN_LED_A, true);
  // delay(500);
  // digitalWrite(POWER_ENABLE_PIN, false);

  sleep_cpu();

}
