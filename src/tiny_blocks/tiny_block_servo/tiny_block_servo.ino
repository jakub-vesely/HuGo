  /*
  BOARD: hugo_adapter (>= M7) + 3V servo
  Chip: ATtiny414
  Clock Speed: 1-10 MHz
  Programmer: jtag2updi (megaTinyCore)
  */

  void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);
  void HugoTinyWireFillModuleVersion();

  #include <hugo_tiny_wire.h>
  #include <hugo_gpio.h>

  #define ADJUSTMENT_VERSION 0

  #define SERVO_PWM_PIN HUGO_PIN_D5
  #define I2C_COMMAND_SET_VALUE 1

  int value = 0;
  bool up = true;
  void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
    switch (command) {
      case I2C_COMMAND_SET_VALUE:
        digitalWrite(SERVO_PWM_PIN, HugoTinyWireRead());
        break;
    }
  }

  void HugoTinyWireFillModuleVersion(){
    s_buffer.data[0] = I2C_BLOCK_TYPE_ID_SERVO;
    s_buffer.data[1] = HUGO_PCB_VERSION;
    s_buffer.data[2] = ADJUSTMENT_VERSION;
    s_buffer.size = 3;
  }

  void HugoTinyWirePowerSave(uint8_t level){
  }

  void setup(){
    HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_BUTTON, NULL);
    pinMode(HUGO_PIN_SHIELD_POWER, OUTPUT);
    digitalWrite(HUGO_PIN_SHIELD_POWER, 1);
    delay(100);
    pinMode(SERVO_PWM_PIN, OUTPUT);
    analogWrite(SERVO_PWM_PIN, 0);
    delay(6000);
    //digitalWrite(HUGO_PIN_SHIELD_POWER, 1);
  }

  void loop(){
    // if (up) {
    //   value+= 10;  
    // }
    // else{
    //   value-= 10;
    // }
    
    // if (value > 253){
    //   value = 253;
    //   up = false;
    // }
    // if (value < 68){
    //   value = 68;
    //   up = true;
    // }

    //analogWrite(SERVO_PWM_PIN, value);//min 55
    //delay(300);
    
    // analogWrite(SERVO_PWM_PIN, 70);//min 55
    // delay(1000);
    // analogWrite(SERVO_PWM_PIN, 140);//min 55
    // delay(1000);
    // analogWrite(SERVO_PWM_PIN, 200); //max 254
    // delay(1000);
    // analogWrite(SERVO_PWM_PIN, 140);//min 55
    // delay(1000);
    
    //sleep_cpu();
    //digitalWrite(HUGO_PIN_SHIELD_POWER, 1);
    //delay(1000);
    //digitalWrite(HUGO_PIN_SHIELD_POWER, 0);
    //delay(1000);
  }
