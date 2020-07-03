/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#include <driver/rmt.h>
#include <driver_rtm_abstraction.h>
#include <esp_log.h>
#include <esp_ringbuffer_abstraction.h>
#include <event_loop.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <hugo_defines.h>
#include <ir_tools.h>
#include <lauxlib.h>
#include <lualib.h>

static int s_ir_remote_pin = 0;
static rmt_channel_t s_rx_channel = RMT_CHANNEL_1;
static lua_State *sL = NULL;
static bool s_stop = false;
static bool s_stopped = false;
static int s_event_id = -1;

static const char *TAG = "ir_remote";

typedef struct
{
    uint32_t addr;
    uint32_t cmd;
    bool repeat;
} ir_data_t;

static ir_parser_t * _initialize_parser()
{
    rmt_config_t rmt_rx_config = RMT_DEFAULT_CONFIG_RX(s_ir_remote_pin, s_rx_channel);
    rmt_config(&rmt_rx_config);
    rmt_driver_install(s_rx_channel, 1000, 0);
    ir_parser_config_t ir_parser_config = IR_PARSER_DEFAULT_CONFIG((ir_dev_t)s_rx_channel);
    ir_parser_config.flags |= IR_TOOLS_FLAGS_PROTO_EXT; // Using extended IR protocols (both NEC and RC5 have extended version)
    ir_parser_config.margin_us = 500;
//#if CONFIG_EXAMPLE_IR_PROTOCOL_NEC
    return  ir_parser_rmt_new_nec(&ir_parser_config);
//#elif CONFIG_EXAMPLE_IR_PROTOCOL_RC5
//    return  ir_parser_rmt_new_rc5(&ir_parser_config);
//#endif
}

static bool _fill_ir_data(RingbufHandle_t rb, ir_parser_t *ir_parser, ir_data_t *out_data)
{
    size_t length = 0;
    bool found = false;
    rmt_item32_t *items = (rmt_item32_t *) X_RINGBUFFER_RECEIVE(rb, &length, 1000);
    //for (int i = 0; i < length; ++i)
    //    printf("{%d, %d, %d, %d},\n", items[i].duration0, items[i].level0, items[i].duration1, items[i].level1);
    //    printf("%u,\n", items[i].val);
    if (items)
    {
        length /= 4; // one RMT = 4 Bytes
        if (ir_parser->input(ir_parser, items, length) == ESP_OK) {
            if (ir_parser->get_scan_code(ir_parser, &(out_data->addr), &(out_data->cmd), &(out_data->repeat)) == ESP_OK)
            {
               found = true;
            }
        }
        //after parsing the data, return spaces to ringbuffer.
        V_RINGBUFFER_RETURN_ITEM(rb, (void *) items);
    }
    return found;
}

static void ir_rx_task(void *arg)
{
    ESP_LOGD(TAG, "ir_rx_task started");
    ir_parser_t *ir_parser = _initialize_parser();

    RingbufHandle_t rb = NULL;
    RMT_GET_RINGBUF_HANDLE(s_rx_channel, &rb);
    ESP_LOGD(TAG, "rb=%d", (int)rb);
    if (rb != NULL)
    {
        RMT_RX_START(s_rx_channel, true);

        ir_data_t ir_data;
        while (!s_stop) {
            if (_fill_ir_data(rb, ir_parser, &ir_data))
            {
                ESP_LOGD(TAG, "Scan Code %s --- addr: 0x%04x cmd: 0x%04x", ir_data.repeat ? "(repeat)" : "", ir_data.addr, ir_data.cmd);
                hugo_raise_event(s_event_id, &ir_data, sizeof(ir_data_t));
            }
        }
    }

    ir_parser->del(ir_parser);
    rmt_driver_uninstall(s_rx_channel);
    ESP_LOGD(TAG, "ir remote process terminated");
    s_stopped = true;

    vTaskDelete(NULL);
}

static void _data_received(event_data_t data, int data_size)
{
    printf("_data_received\n");
    ir_data_t *ir_data = (ir_data_t *)data;
    lua_getglobal(sL, "Ir_remote_callback");
    lua_pushinteger(sL, ir_data->addr);
    lua_pushinteger(sL, ir_data->cmd);
    lua_pushinteger(sL, ir_data->repeat);

    if (lua_pcall(sL, 3, 0, 0))
    {
        ESP_LOGE(TAG, "Ir_remote_callback pcall failed: %s\n", lua_tostring(sL, -1));
    }
}

void hugo_ir_remote_init_module(lua_State* L, int ir_remote_pin)
{
    sL = L;
    s_ir_remote_pin = ir_remote_pin;
    s_event_id = hugo_get_new_event_id();
    hugo_add_event_action(s_event_id, _data_received);
}

void hugo_ir_remote_start_listening()
{
    s_stop = false;
    s_stopped = false;
    xTaskCreate(ir_rx_task, "ir_rx_task", 8192, NULL, 10, NULL);
}

void hugo_ir_remote_stop_listening()
{
    s_stop = true;
    while (!s_stopped) //TODO: timeout?
    {
        vTaskDelay(10);
    }
}
