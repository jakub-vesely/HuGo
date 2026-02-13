#include <Arduino.h>

#include "tiny_ble_shield.h"
#include <tiny_main_base.h>
#include <hugo_defines.h>
#include <hugo_gpio.h>
#include <hugo_utils.h>

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);
void HugoTinyWireFillModuleVersion();

#define WAKEUP_PIN HUGO_PIN_D3
#define MESH_DATA_SIZE 12 // max size of mesh data (16 including 2B_sender 2B_receiver)
#define READ_BUFFER_SIZE (MESH_HEADER_LENGTH + MESH_DATA_SIZE) //mesh header + mesh_data

#define PCB_VERSION 0
#define ADJUSTMENT_VERSION 0

#define I2C_AT_COMMAND_REQ   0x01
#define I2C_AT_COMMAND_RESP  0x02
#define I2C_MESH_DATA        0x03
#define AT_MESH_PREFIX_SIZE 7

static tiny_common_buffer_t* p_common_buffer = tiny_main_base_get_common_buffer();
uint8_t temp_buffer[MESH_DATA_SIZE];
uint8_t stored_buffer_size = 0;

void data_received(){
//just to wake up
}

void BleShield::init(int32_t baudrate){
//FIXME:it is used PORTMUX.CTRLB for older versions instead of swap
//can be affected pin mode of PB2
#if defined(__AVR_ATtiny824__) || defined(__AVR_ATtiny1624__) || defined(__AVR_ATtiny3224__)
  Serial.swap(1);
#endif
  Serial.begin(baudrate);

  pinMode(WAKEUP_PIN, OUTPUT);
  digitalWrite(WAKEUP_PIN, HIGH); //high is non-active


#if HUGO_PCB_VERSION > 7 //BLE pcb must be in version > 7, adapter version > 6
  attachInterrupt(digitalPinToInterrupt(HUGO_PIN_D5), data_received, RISING);
#endif

  this->_read_jdy(500); //wait for JDY-25-START
}

// read line from JDY to ble_buffer and process it
void BleShield::_read_jdy(uint16_t timeout){
  p_common_buffer->size = 0;
  uint8_t expected_message_length = 0;
  unsigned long timeout_start = millis();
  while (true){
    if (millis() > timeout_start + timeout){
      p_common_buffer->size = 0; // no complete message has been read
      break;
    }
    if (Serial.available() == 0){
      continue;
    }

    uint8_t byte = Serial.read();

    if (p_common_buffer->size == COMMON_BUFFER_SIZE){
      // too long line to be stored it seems the message is corrupted will be deleted for sure
      p_common_buffer->size = 0;
      continue;
    }

    if (p_common_buffer->size == 0 && byte != '+' && byte != 'O' && byte != 0xf1){
      continue; //we are probably lost somewhere in the middle of the message. waiting for the correct start
    }
    if (p_common_buffer->size == 1 && p_common_buffer->data[0] == 'O' && byte != 'K'){
      p_common_buffer->size = 0;
      continue; //O was not from OK
    }

    //mesh message contains length as a last byte of header
    //to be possible to transfer binary data, checking \r \n must be avoided until end of expected data
    if (p_common_buffer->size == 2 && p_common_buffer->data[0] == 0xf1){ //0xF1 0xDD 1B_size
      expected_message_length = byte;
    }
    else if (expected_message_length > 0){
      if (--expected_message_length == 0) {
        p_common_buffer->data[p_common_buffer->size++] = byte;
        break;
      }
    }
    else if (p_common_buffer->size > 0 && p_common_buffer->data[0] != 0xf1){
      //will not be stored waiting for \n
      if (byte == '\r'){ //empty mesh message is not expected
        continue;
      }

      //line complete
      if (byte == '\n'){
        break;
      }
    }

    p_common_buffer->data[p_common_buffer->size++] = byte;
  }
}

void BleShield::command_send_and_receive(uint8_t *data, uint8_t size, uint16_t timeout){

  for (uint8_t index = 0; index < size; index++){
      Serial.write(data[index]);
  }
  Serial.write('\r');
  Serial.write('\n');

  this->_read_jdy(timeout);
}

void BleShield::command_send_and_receive(uint16_t timeout){
  this->command_send_and_receive(p_common_buffer->data, p_common_buffer->size, timeout);
}

void BleShield::send_response_flag(uint8_t sender_low){

  p_common_buffer->size = 5;
  p_common_buffer->data[0] = 0xAA;
  p_common_buffer->data[1] = 0xB1;
  p_common_buffer->data[2] = 0xE7;
  p_common_buffer->data[3] = 0x01; //set output 1
  p_common_buffer->data[4] = 0x01; //to Hi

  this->send_mesh_message(sender_low, 0x10); //set output of mesh target
}

uint16_t BleShield::get_mesh_message(uint8_t timeout){
#define MESH_RECV_HEADER_LENGTH 7 //0xF1 0xDD 1B_size 2B_sender 2B_receiver
  this->_read_jdy(timeout);

  if (p_common_buffer->size < MESH_RECV_HEADER_LENGTH || p_common_buffer->data[0] != 0xf1){
    return 0;
  }

  uint16_t sender_id = p_common_buffer->data[4];
  sender_id += p_common_buffer->data[3] >> 8;

  bool has_flag_byte = false;
  uint8_t checksum = 0;
  uint8_t stored_checksum = 0;
  uint8_t flag_byte = p_common_buffer->data[MESH_RECV_HEADER_LENGTH];
  if (flag_byte & 0b10000000){ //first byte is the flag byte
    has_flag_byte = true;
    stored_checksum = p_common_buffer->data[p_common_buffer->size-1];
    p_common_buffer->size -= 1; //checksum
  }

  for (uint8_t index = 0; index < p_common_buffer->size - MESH_RECV_HEADER_LENGTH; index++){
    p_common_buffer->data[index]  = p_common_buffer->data[index + MESH_RECV_HEADER_LENGTH];
    if (has_flag_byte){
      checksum ^= p_common_buffer->data[index];
    }
  }
  p_common_buffer->size = p_common_buffer->size - MESH_RECV_HEADER_LENGTH;

  if (checksum != stored_checksum ){ //both are 0 without flag_byte
    return 100;//0;
  }

  return sender_id;
}


void BleShield::power_save(bool save){
  if (save){
    tiny_main_string_to_buffer("AT+SLEEP2");
    this->command_send_and_receive(); //resp: OK
    this->_read_jdy(300); //resp: +SLEEP
    this->_sleeping = true;
  }
  else{
    digitalWrite(WAKEUP_PIN, LOW);
    delay(50);
    digitalWrite(WAKEUP_PIN, HIGH);

    this->_read_jdy(400); //wait to wake up is finished
    this->_sleeping = false;
  }
}
bool BleShield::send_mesh_message(uint8_t target_id, uint8_t type_id){

#define AT_MESH_PREFIX_COMMAND_TARGET_SIZE (AT_MESH_PREFIX_SIZE + 3)
#define TAIL_SIZE 2

  if (AT_MESH_PREFIX_COMMAND_TARGET_SIZE + p_common_buffer->size + TAIL_SIZE > COMMON_BUFFER_SIZE){
    return false;
  }

  for (int8_t index = p_common_buffer->size - 1; index >= 0; index--){
    p_common_buffer->data[index + AT_MESH_PREFIX_COMMAND_TARGET_SIZE] = p_common_buffer->data[index];
  }
  p_common_buffer->size += AT_MESH_PREFIX_COMMAND_TARGET_SIZE;
  memcpy(p_common_buffer->data, "AT+MESH", AT_MESH_PREFIX_SIZE);

  //command 1B
  p_common_buffer->data[7] = type_id;

  //target id
  p_common_buffer->data[8] = 0x00; //higher byte of target_id
  p_common_buffer->data[9] = target_id;

  this->command_send_and_receive();
  return true;
}

bool BleShield::send_mesh_data(uint8_t target_id, bool acq_expected){
#if HUGO_PCB_VERSION < 8 //BLE pcb must be in version > 7, adapter version > 6
  acq_expected = false;
#endif

  if (acq_expected){
    memcpy(temp_buffer, p_common_buffer->data, MESH_DATA_SIZE);
    stored_buffer_size = p_common_buffer->size;
    // when message is send, OK is returned by jdy to the common_buffer but another message can be delivered as well
    // the original message has to be preserved and then restored
    this->command_send_and_receive((uint8_t*)"AT+CUIO1,0", 10); //reset response pin
    memcpy(p_common_buffer->data, temp_buffer, MESH_DATA_SIZE);
    p_common_buffer->size = stored_buffer_size;
  }

  uint8_t checksum = 0;
  for (int8_t index = p_common_buffer->size - 1; index >= 0; index--){
    checksum ^= p_common_buffer->data[index];
    p_common_buffer->data[index + 1] = p_common_buffer->data[index];
  }

  // flag byte:
  // 7:   1 message contains flag byte and checksum. 0 is message without flag and checksum bytes in plain text
  // 6:   1 means acknowledge  expected, 0 means no acq message expected
  // 4-5: message format: 00 means plain text - others not used yet
  // 0-3: message ID

  p_common_buffer->data[0] = 0b10000000 | (this->message_id++ & 0b00001111);
  if (acq_expected){
    p_common_buffer->data[0] |= 0b01000000;
  }
  checksum ^= p_common_buffer->data[0];
  p_common_buffer->data[p_common_buffer->size + 1] = checksum ;
  p_common_buffer->size += 2; //id + checksum

  if (acq_expected){ //acq expected will not be valid vor version < 8
#if HUGO_PCB_VERSION > 7 //BLE pcb must be in version > 7, adapter version > 6
    uint8_t countdown = 5; //message doesn't have to be delivered or answer doesnt has be delivered 3 could be enough 5 is for sure
    memcpy(temp_buffer, p_common_buffer->data, MESH_DATA_SIZE);
    stored_buffer_size = p_common_buffer->size;
    while (countdown-- > 0){
      if (!this->send_mesh_message(target_id, 0x00)){ //command - 0 message without ack; 1 with ack
        return false; //message is too long to be send
      }
      sleep_for_250ms();
      if (digitalRead(HUGO_PIN_D5)){
        return true; //message receive confirmed
      }

      // when message is send, OK is returned by jdy to the common_buffer but another message can be delivered as well
      // the original message has to be preserved and then restored
      memcpy(p_common_buffer->data, temp_buffer, MESH_DATA_SIZE);
      p_common_buffer->size = stored_buffer_size;
    }
#endif
    return false;
  }
  else {
    return this->send_mesh_message(target_id, 0x00); //receive pin is not implemented
  }
}
bool BleShield::has_message_flag_byte(){
    return p_common_buffer->data[0] & 0b10000000;
}

bool BleShield::is_acq_expected(){
    return p_common_buffer->data[0] & 0b01000000;
}

bool BleShield::is_message_in_plain_text(){
    return (p_common_buffer->data[0] & 0b00110000) == 0;
}
uint8_t BleShield::get_message_id(){
    return p_common_buffer->data[0] & 0b00001111;
}

