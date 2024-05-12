#include "tiny_ble_shield.h"
#include <tiny_main_base.h>
#include <hugo_defines.h>

#include <Arduino.h>

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);
void HugoTinyWireFillModuleVersion();

#define WAKEUP_PIN PIN_PA3
#define COMMAND_RESPONSE_SIZE 16 //largest 13 (+JDY-25-START)

#define MESH_DATA_SIZE 12 // max size of mesh data (16 including 2B_sender 2B_receiver)
#define READ_BUFFER_SIZE (MESH_HEADER_LENGTH + MESH_DATA_SIZE) //mesh header + mesh_data

#define PCB_VERSION 0
#define ADJUSTMENT_VERSION 0

#define I2C_AT_COMMAND_REQ   0x01
#define I2C_AT_COMMAND_RESP  0x02
#define I2C_MESH_DATA        0x03

static tiny_common_buffer_t* p_common_buffer = tiny_main_base_get_common_buffer();

static uint16_t s_exp_resp_timeout_ms = 0;
static unsigned long s_exp_resp_timeout_start = 0;

void BleShield::init(){
#if defined(__AVR_ATtiny414__) || defined(__AVR_ATtiny1614__)
  pinMode(PIN_PA6, INPUT); //to be used PB3 and PB2 instead
  pinMode(PIN_PA7, INPUT);
#endif
  Serial.begin(115200);

  pinMode(WAKEUP_PIN, OUTPUT);
  digitalWrite(WAKEUP_PIN, HIGH); //high is non-active

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
      continue;; //we are probably lost somewhere in the middle of the message. waiting for the correct start
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

void BleShield::command_send_and_receive(uint16_t timeout){
  for (uint8_t index = 0; index < p_common_buffer->size; index++){
      Serial.write(p_common_buffer->data[index]);
  }
  Serial.write('\r');
  Serial.write('\n');

  this->_read_jdy(timeout);
}

uint16_t BleShield::get_mesh_message(uint8_t timeout){
#define MESH_RECV_HEADER_LENGTH 7 //0xF1 0xDD 1B_size 2B_sender 2B_receiver
  this->_read_jdy(timeout);

  if (p_common_buffer->size < MESH_RECV_HEADER_LENGTH || p_common_buffer->data[0] != 0xf1){
    return -1;
  }

  uint16_t sender_id = p_common_buffer->data[4];
  sender_id += p_common_buffer->data[3] >> 8;

  for (uint8_t index = 0; index < p_common_buffer->size - MESH_RECV_HEADER_LENGTH; index++){
    p_common_buffer->data[index]  = p_common_buffer->data[index] + MESH_RECV_HEADER_LENGTH;
  }
  p_common_buffer->size = p_common_buffer->size - MESH_RECV_HEADER_LENGTH;
  return sender_id;
}

void BleShield::power_save(bool save){
  if (save){
    tiny_main_string_to_buffer("AT+SLEEP2");
    this->command_send_and_receive(COMMON_BLE_TIMEOUT_MS); //resp: OK
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

void BleShield::send_mesh_data(uint8_t target_id){
#define AT_MESH_PREFIX_SIZE 7
#define AT_MESH_PREFIX_COMMAND_TARGET_SIZE (AT_MESH_PREFIX_SIZE + 3)
#define TAIL_SIZE 2

  if (AT_MESH_PREFIX_COMMAND_TARGET_SIZE + p_common_buffer->size + TAIL_SIZE > COMMON_BUFFER_SIZE){
    return;
  }

  for (int8_t index = p_common_buffer->size - 1; index >= 0; index--){
    p_common_buffer->data[index + AT_MESH_PREFIX_COMMAND_TARGET_SIZE] = p_common_buffer->data[index];
  }
  p_common_buffer->size += AT_MESH_PREFIX_COMMAND_TARGET_SIZE;
  memcpy(p_common_buffer->data, "AT+MESH", AT_MESH_PREFIX_SIZE);

  //command 1B
  p_common_buffer->data[7] = 0x01; //command - 0 message without ack; 1 with ack

  //target id
  p_common_buffer->data[8] = 0x00; //higher byte of target_id
  p_common_buffer->data[9] = target_id;

  this->command_send_and_receive(1000);
}
