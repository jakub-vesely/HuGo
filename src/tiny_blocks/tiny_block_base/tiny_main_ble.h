#pragma once
#include <string.h>
#include <hugo_defines.h>
#include <tiny_main_base.h>

#define AT_COMMAND_ID 0x01
#define RESPONSE_COMMAND_ID = 0x02
#define MESH_DATA_ID = 0x03

uint8_t tiny_main_ble_mesh_message[16];
uint8_t tiny_main_ble_mesh_message_size = 0;

void tiny_main_ble_send_mesh_data(uint8_t target_id){
  tiny_common_buffer_t* buffer = tiny_main_base_get_common_buffer();
  buffer->size = 0;

  memcpy(buffer->data + buffer->size, "AT+MESH", 7);
  buffer->size += 7;

  buffer->data[buffer->size++] = 0x00; //message without ack
  buffer->data[buffer->size++] = 0x00; //higher byte of target_id
  buffer->data[buffer->size++] = target_id;

  memcpy(buffer->data + buffer->size, tiny_main_ble_mesh_message, tiny_main_ble_mesh_message_size);
  buffer->size += tiny_main_ble_mesh_message_size;

  buffer->data[buffer->size++] = 0x0d;
  buffer->data[buffer->size++] = 0x0a;
  buffer->data[buffer->size++] = '\r';
  buffer->data[buffer->size++] = '\n';

  tiny_main_base_send_i2c_command(I2C_BLOCK_TYPE_ID_BLE, I2C_BLOCK_TYPE_ID_BLE, AT_COMMAND_ID, 0xff);
}
