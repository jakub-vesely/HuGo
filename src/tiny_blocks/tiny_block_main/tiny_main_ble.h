#pragma once

#ifdef main_ble
#include <string.h>
#include <hugo_defines.h>
#include <tiny_main_base.h>

#define BLE_I2C_AT_COMMAND_REQ   0x01
#define BLE_I2C_AT_COMMAND_RESP  0x02
#define BLE_I2C_MESH_DATA        0x03

#define MAIN_BLE_MESH_BUFFER_SIZE 12
typedef struct tiny_main_ble_mesh_message_t {
  uint8_t data[MAIN_BLE_MESH_BUFFER_SIZE];
  uint8_t size;
};

tiny_main_ble_mesh_message_t tiny_main_ble_mesh_message;

/**
 * @brief data are expected in the common_buffer
 *
 * @param block_id
 */
void tiny_main_ble_send_command(uint8_t block_id){
  tiny_main_base_send_i2c_command(block_id, I2C_BLOCK_TYPE_ID_BLE, BLE_I2C_AT_COMMAND_REQ, 0);
}

/**
 * @brief incomming data are filled to the common_buffer
 *
 * @param block_id
 */
bool tiny_main_ble_get_command_response(uint8_t block_id){
  return tiny_main_base_send_i2c_command(block_id, I2C_BLOCK_TYPE_ID_BLE, BLE_I2C_AT_COMMAND_RESP, 0xff);
}


/**
 * @brief incomming data are filled to the common_buffer
 *
 * @param block_id
 */
bool tiny_main_ble_get_mesh_message(uint8_t block_id){
  return tiny_main_base_send_i2c_command(block_id, I2C_BLOCK_TYPE_ID_BLE, BLE_I2C_MESH_DATA, 0xff);
}

/**
 * @brief data are expected in tiny_main_ble_mesh_message
 *
 * @param block_id
 */
void tiny_main_ble_send_mesh_data(uint8_t block_id,  uint8_t target_id){
  tiny_common_buffer_t* buffer = tiny_main_base_get_common_buffer();

  buffer->size = 0;
  memcpy(buffer->data, "AT+MESH", 7);
  buffer->size += 7;

  buffer->data[buffer->size++] = 0x00; //message without ack
  buffer->data[buffer->size++] = 0x00; //higher byte of target_id
  buffer->data[buffer->size++] = target_id;

  memcpy(buffer->data + buffer->size, tiny_main_ble_mesh_message.data, tiny_main_ble_mesh_message.size);
  buffer->size += tiny_main_ble_mesh_message.size;
  tiny_main_ble_mesh_message.size = 0;

  buffer->data[buffer->size++] = 0x0d;
  buffer->data[buffer->size++] = 0x0a;
  tiny_main_ble_send_command(block_id);
}

#endif //main_ble
