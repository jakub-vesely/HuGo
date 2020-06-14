/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

// #include "esp_log.h"
// #include <driver/mcpwm.h>
#include <hugo_defines.h>

#include  <lualib.h>
// #include <external/lua/src/lauxlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/rmt.h"
#include <ir_tools.h>
#include "esp_log.h"

#define IR_REMOTE_PIN 19

static rmt_channel_t rx_channel = RMT_CHANNEL_1;
static lua_State *sL = NULL;

static const char *TAG = "IR_REMOTE";

//taken from ir_protocols_example
static void ir_rx_task(void *arg)
{
    uint32_t addr = 0;
    uint32_t cmd = 0;
    uint32_t length = 0;
    bool repeat = false;
    RingbufHandle_t rb = NULL;
    rmt_item32_t *items = NULL;

    rmt_config_t rmt_rx_config = RMT_DEFAULT_CONFIG_RX(IR_REMOTE_PIN, rx_channel);
    rmt_config(&rmt_rx_config);
    rmt_driver_install(rx_channel, 1000, 0);
    ir_parser_config_t ir_parser_config = IR_PARSER_DEFAULT_CONFIG((ir_dev_t)rx_channel);
    ir_parser_config.flags |= IR_TOOLS_FLAGS_PROTO_EXT; // Using extended IR protocols (both NEC and RC5 have extended version)
    ir_parser_config.margin_us = 500;
    ir_parser_t *ir_parser = NULL;

//#if CONFIG_EXAMPLE_IR_PROTOCOL_NEC
    ir_parser = ir_parser_rmt_new_nec(&ir_parser_config);
//#elif CONFIG_EXAMPLE_IR_PROTOCOL_RC5
//    ir_parser = ir_parser_rmt_new_rc5(&ir_parser_config);
//#endif

    //get RMT RX ringbuffer
    rmt_get_ringbuf_handle(rx_channel, &rb);
    // Start receive
    rmt_rx_start(rx_channel, true);
    while (rb) {
        items = (rmt_item32_t *) xRingbufferReceive(rb, &length, 10000);
        if (items) {
            length /= 4; // one RMT = 4 Bytes
            if (ir_parser->input(ir_parser, items, length) == ESP_OK) {
                if (ir_parser->get_scan_code(ir_parser, &addr, &cmd, &repeat) == ESP_OK) {
                    ESP_LOGI(TAG, "Scan Code %s --- addr: 0x%04x cmd: 0x%04x", repeat ? "(repeat)" : "", addr, cmd);

                    lua_getglobal(sL, "Ir_remote_callback");
                    lua_pushinteger(sL, cmd);
                    lua_pushinteger(sL, repeat);
                    if (lua_pcall(sL, 2, 0, 0))
                        ESP_LOGE(TAG, "Ir_remote_callback pcall failed: %s\n", lua_tostring(sL, -1));

                }
            }
            //after parsing the data, return spaces to ringbuffer.
            vRingbufferReturnItem(rb, (void *) items);
        } else {
            break;
        }
    }
    printf("process terminated");
    ir_parser->del(ir_parser);
    rmt_driver_uninstall(rx_channel);
    vTaskDelete(NULL);
}

void hugo_ir_remote_init_module(lua_State* L)
{
    sL = L;
    xTaskCreate(ir_rx_task, "ir_rx_task", 32768, NULL, 10, NULL);
}
