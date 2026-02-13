#pragma once

#include <stdint.h>

#define COMMON_BLE_TIMEOUT_MS 200

class BleShield{

    void _read_jdy(uint16_t timeout);

    bool _sleeping = false;
    uint8_t message_id=0;

public:
    void init(int32_t baudrate=115200); //factory default is 9600 but used one in 115200
    void command_send_and_receive(uint8_t *data, uint8_t size, uint16_t timeout=COMMON_BLE_TIMEOUT_MS);
    void command_send_and_receive(uint16_t timeout=COMMON_BLE_TIMEOUT_MS);

    void power_save(bool save);
    bool send_mesh_message(uint8_t target_id, uint8_t type_id);
    bool send_mesh_data(uint8_t target_id, bool acq_expected=false);
    uint16_t get_mesh_message(uint8_t timeout=COMMON_BLE_TIMEOUT_MS);
    bool is_sleeping(){ return _sleeping;}
    void send_response_flag(uint8_t sender_low);

    bool has_message_flag_byte();
    bool is_acq_expected();
    bool is_message_in_plain_text();
    uint8_t get_message_id();
};
