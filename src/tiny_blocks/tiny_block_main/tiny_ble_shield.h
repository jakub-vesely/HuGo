#pragma once

#include <stdint.h>

#define COMMON_BLE_TIMEOUT_MS 200

class BleShield{

    void _read_jdy(uint16_t timeout);
    bool _sleeping = false;

public:
    void init(int baudrate=115200); //factory default is 9600 but used one in 115200
    void command_send_and_receive(uint16_t timeout=COMMON_BLE_TIMEOUT_MS);
    void power_save(bool save);
    void send_mesh_data(uint8_t target_id);
    uint16_t get_mesh_message(uint8_t timeout=COMMON_BLE_TIMEOUT_MS);
    bool is_sleeping(){ return _sleeping;}
};
