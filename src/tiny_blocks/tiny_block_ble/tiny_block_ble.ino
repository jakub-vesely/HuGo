/*
BOARD: hugo_adapter + hugo_rgb
Chip: ATtiny1614
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

#include <stdint.h>
void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);
void HugoTinyWireFillModuleVersion();

#include <Arduino.h>

#include <hugo_tiny_wire.h>
#include <avr/sleep.h>
#include <Watchdog.h>
#include <avr/wdt.h>

#define PCB_VERSION 0
#define ADJUSTMENT_VERSION 0

#define MESH_HEADER_LENGHT 7
#define COMMAND_RESPONSE_SIZE 16
#define MESH_DATA_SIZE 16 // max size of mesh data
#define READ_BUFFER_SIZE (MESH_HEADER_LENGHT + MESH_DATA_SIZE) //mesh header + mesh_data 

#define I2C_AT_COMMAND_REQ   0x01
#define I2C_AT_COMMAND_RESP  0x02
#define I2C_MESH_DATA        0x03

#define WAKEUP_PIN PIN_PA3

typedef struct read_buffer_t {
  uint8_t data[READ_BUFFER_SIZE];
  uint8_t size;
  bool complete;
} read_buffer_t;

typedef struct command_response__t {
  uint8_t data[COMMAND_RESPONSE_SIZE];
  uint8_t size;
} command_response__t;

typedef struct mesh_message_t {
  uint8_t sender_id;
  uint8_t data[MESH_DATA_SIZE];
  uint8_t size;
} mesh_message_t;

static read_buffer_t s_read_buffer;
static command_response__t s_command_response_buffer;
static mesh_message_t s_mesh_message;

typedef struct flags_t { 
  bool wake_jdy : 1;
  bool sleep_jdy: 1;
  bool sleep_me : 1;
  bool new_line_expected : 1;
  unsigned expected_resp_count : 4;
} flags_t;

static flags_t s_flags = { false, false, false, false, 0};

static uint16_t s_exp_resp_timeout_ms = 0;
static unsigned long s_exp_resp_timeout_start = 0;

//Watchdog watchdog;

void wakeup(){
  digitalWrite(WAKEUP_PIN, LOW);
  delay(20);
  digitalWrite(WAKEUP_PIN, HIGH);
}

void process_complete_buffer(){
  if (s_read_buffer.size >= MESH_HEADER_LENGHT && s_read_buffer.data[0] == 0xf1){
    if (s_mesh_message.size == 0){
      s_mesh_message.size = s_read_buffer.size - MESH_HEADER_LENGHT;
      memcpy(s_mesh_message.data, s_read_buffer.data + MESH_HEADER_LENGHT, s_mesh_message.size);
      s_mesh_message.sender_id = s_read_buffer.data[4];
      s_read_buffer.size = 0;
      s_read_buffer.complete  = false;
    }
  }
  else if (
      (s_read_buffer.size == 2 && s_read_buffer.data[0] == 'O' && s_read_buffer.data[1] == 'K') ||
      (s_read_buffer.size > 1 && s_read_buffer.data[0] == '+')
  ){
    if (s_command_response_buffer.size == 0){
      memcpy(s_command_response_buffer.data, s_read_buffer.data, s_read_buffer.size);
      s_command_response_buffer.size = s_read_buffer.size;
      s_read_buffer.size = 0;
      s_read_buffer.complete  = false;
    }
    return;
  }
  else{ //unknown message - do not know how to process it but I need to free space for next messages - will be deleted
    s_read_buffer.size = 0;
    s_read_buffer.complete  = false;
  }
}

// read line from JDY to ble_buffer and process it 
void readJdy(){
  while (true){
    //previous data not processed yet
    if (s_read_buffer.complete){
      process_complete_buffer();
    }

    if (Serial.available() == 0){
      break;
    }

    uint8_t byte = Serial.read();

    //will not be stored waiting for \n
    if (byte == '\r'){
      s_flags.new_line_expected = true; // to prevent case when \n (10) is present in mesh as the message size
      continue;
    }

    //line complete
    if (s_flags.new_line_expected && byte == '\n'){
      s_read_buffer.complete = true;
      s_flags.new_line_expected = false;
      continue;
    }

    // too long line to be stored but it is necessary to finish reading to
    // to doesn't be stored rest of the line as a new message
    if (s_read_buffer.size == READ_BUFFER_SIZE){
      continue;
    }

    s_read_buffer.data[s_read_buffer.size++] = byte;
  }

}

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
  switch (command) {
    case I2C_AT_COMMAND_REQ:
      for (uint8_t index = 0; index < payload_size; index++){
        Serial.write(HugoTinyWireRead());
      }
      Serial.write("\r\n");
      break;

    case I2C_AT_COMMAND_RESP:
      if (s_flags.expected_resp_count > 0){ //command response will be processed internally
        s_buffer.data[0] = 0;
        s_buffer.size = 1;
      }
      else{
        s_buffer.data[0] = s_command_response_buffer.size;
        memcpy(s_buffer.data + 1, s_command_response_buffer.data, s_command_response_buffer.size);
        s_buffer.size = s_command_response_buffer.size + 1;
        s_command_response_buffer.size = 0;
      }
      break;
    case I2C_MESH_DATA:
      if (s_mesh_message.size){
        s_buffer.data[0] = s_mesh_message.size + 1; //sender_id + message
        s_buffer.data[1] = s_mesh_message.sender_id; 
        memcpy(s_buffer.data + 2, s_mesh_message.data, s_mesh_message.size);
        s_buffer.size = s_mesh_message.size + 2;
        s_mesh_message.size = 0;
      }
      else{
        s_buffer.data[0] = 0;
        s_buffer.size = 1;
      }
      break;
  }
}

void HugoTinyWireFillModuleVersion(){
  s_buffer.data[0] = I2C_BLOCK_TYPE_ID_BLE;
  s_buffer.data[1] = PCB_VERSION;
  s_buffer.data[2] = ADJUSTMENT_VERSION;
  s_buffer.size = 3;
}

void HugoTinyWirePowerSave(uint8_t level){
 if (level == POWER_SAVE_NONE){
   s_flags.wake_jdy = true;
   return;
 }

 if (level >= POWER_SAVE_LIGHT){
   s_flags.sleep_jdy = true;
 } 
 if (level == POWER_SAVE_DEEP){
   s_flags.sleep_me = true;
 }

}

void setup()
{
  HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_BLE, NULL);

  Serial.begin(9600);
  s_read_buffer.size = 0;
  s_read_buffer.complete = false;
  s_command_response_buffer.size = 0;
  s_mesh_message.size = 0;

  pinMode(WAKEUP_PIN, OUTPUT);
  digitalWrite(WAKEUP_PIN, HIGH); //high is non-active

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  //pinMode(PIN_PA6, INPUT); //to be used PB3 and PB2 instead
  //pinMode(PIN_PA7, INPUT);

  //watchdog.enable(Watchdog::TIMEOUT_120MS);
  //_PROTECTED_WRITE(WDT.CTRLA,0);
}

// the loop function runs over and over again forever
void set_expected_resp_count(uint8_t count, uint16_t timeout_ms){
  s_flags.expected_resp_count = count;
  s_exp_resp_timeout_ms = timeout_ms;
  s_exp_resp_timeout_start = millis();
}
void loop()
{
  readJdy();
  
  if (s_flags.expected_resp_count > 0 && millis() > s_exp_resp_timeout_start + s_exp_resp_timeout_ms){
    s_flags.expected_resp_count = 0;
  }

  //remove responses that belong to mesages send internally
  if (s_flags.expected_resp_count > 0 && s_command_response_buffer.size > 0){
    s_command_response_buffer.size = 0; //data are not verified, not clear what to do if are unexpected
    s_flags.expected_resp_count--;
  } 
  
  if (s_flags.sleep_jdy){
    set_expected_resp_count(2, 200); //OK + +SLLEP
    Serial.write("AT+SLEEP2");
    s_flags.sleep_jdy = false;

  }

  if (s_flags.wake_jdy){
    set_expected_resp_count(1, 300); //+WAKE
    wakeup();
    s_flags.wake_jdy = false;
  }

  if (s_flags.sleep_me){
    sleep_mode();
    s_flags.sleep_me = false;
  }
  
  //wdt_disable();  // disable watchdog when running
  //watchdog.reset();
}
