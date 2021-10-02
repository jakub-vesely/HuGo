/*
 * Copyright (c) 2021 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

//based on example_ble_sec_gatts_demo.c and gatts_demo.c from esp idf examples
#include "ble.h"

#include <stdbool.h>
#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_system.h>
#include <esp_log.h>
#include <esp_bt.h>
#include <esp_bt_device.h>
#include <esp_gap_ble_api.h>
#include <esp_gatts_api.h>
#include <esp_bt_defs.h>
#include <esp_bt_main.h>

#include <event_loop.h>

#define TAG           "BLE"
#define APP_ID        (0x55) //taken from example
static uint8_t manufacturer[4] = {'H', 'U', 'G', 'O'};
#define GET_PROPERTY_MAX_LEN  (sizeof(uint8_t))
#define SET_PROPERTY_MAX_LEN  (sizeof(uint8_t) + sizeof(double))
#define COMMAND_MAX_LEN  (500)
#define CHAR_DECLARATION_SIZE (sizeof(uint8_t))
#define ADV_CONFIG_FLAG       (1 << 0)
#define SCAN_RSP_CONFIG_FLAG  (1 << 1)
#define DEVICE_NAME_PREFIX_LEN (12)
#define DATA_FOR_COMMAND_SIZE (12)

#define SERVICE_DEFINITION() { \
  {ESP_GATT_AUTO_RSP}, { \
    ESP_UUID_LEN_16, \
    (uint8_t *)&s_primary_service_uuid, \
    ESP_GATT_PERM_READ, \
    ESP_UUID_LEN_32, \
    sizeof(s_service_id), \
    (uint8_t *)&s_service_id \
  } \
}

#define CHARACTERISTIC_DEFINITION() { \
  {ESP_GATT_AUTO_RSP}, { \
    ESP_UUID_LEN_16, \
    (uint8_t *)&s_character_declaration_uuid, \
    ESP_GATT_PERM_READ, \
    CHAR_DECLARATION_SIZE, \
    CHAR_DECLARATION_SIZE, \
    (uint8_t *)&s_char_prop_write_notify \
  } \
}

#define VALUE_DEFINITION(uuid, max_len) { \
  {ESP_GATT_RSP_BY_APP}, { \
    ESP_UUID_LEN_32, \
    (uint8_t *)&uuid, \
    ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, \
    max_len, \
    0, \
    NULL \
  } \
}

#define CFG_DEFINITION()  { \
  {ESP_GATT_AUTO_RSP}, { \
    ESP_UUID_LEN_16, \
    (uint8_t *)&s_character_client_config_uuid, \
    ESP_GATT_PERM_READ|ESP_GATT_PERM_WRITE, \
    sizeof(uint16_t), \
    sizeof(s_ccc), \
    (uint8_t *)s_ccc \
  } \
} \

typedef struct {
  //up to EVENT_DATA_SIZE
  uint8_t property_id;
  esp_gatt_if_t gatts_if;
  uint16_t conn_id;
} get_value_event_data_t;

typedef struct {
  //up to EVENT_DATA_SIZE
  uint8_t property_id;
  esp_gatt_if_t gatts_if;
  uint16_t conn_id;
  double value;
} set_value_event_data_t;

typedef struct {
  //up to EVENT_DATA_SIZE
  uint8_t command_id;
  esp_gatt_if_t gatts_if;
  uint16_t conn_id;
  uint8_t data[DATA_FOR_COMMAND_SIZE];
} command_with_data_t;

///Attributes State Machine
enum
{
    ASM_IDX_SVC,
    ASM_IDX_GET_PROPERTY_CHAR,
    ASM_IDX_GET_PROPERTY_VAL,
    ASM_IDX_GET_PROPERTY_CFG,
    ASM_IDX_SET_PROPERTY_CHAR,
    ASM_IDX_SET_PROPERTY_VAL,
    ASM_IDX_SET_PROPERTY_CFG,
    ASM_IDX_COMMAND_CHAR,
    ASM_IDX_COMMAND_VAL,
    ASM_IDX_COMMAND_CFG,
    ASM_IDX_END,
};

static lua_State *s_L = NULL;
static command_callback_t s_command_callback;

static bool s_secure = true;
static int s_get_value_event_id = -1;
static int s_set_value_event_id = -1;
static int s_command_event_id = -1;

static const uint8_t s_ccc[2] = {0x00, 0x00}; //taken from example
static uint8_t s_adv_config_done = 0;

static const uint16_t s_primary_service_uuid = ESP_GATT_UUID_PRI_SERVICE;
static const uint16_t s_character_declaration_uuid = ESP_GATT_UUID_CHAR_DECLARE;
static const uint16_t s_character_client_config_uuid = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;

static char s_device_name_prefix[DEVICE_NAME_PREFIX_LEN]; //will be assigned during initialization

static uint8_t s_service_uuid[16] = { //taken from example
    /* LSB <--------------------------------------------------------------------------------> MSB */
    //first uuid, 16bit, [12],[13] is the value
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const uint32_t s_service_id =          0x4875476F; //HuGo
static const uint32_t s_get_property_uuid =   0x48754770;
static const uint32_t s_set_property_uuid =   0x48754771;
static const uint32_t s_command_uuid =        0x48754772;

static const uint8_t s_char_prop_write_notify = ESP_GATT_CHAR_PROP_BIT_WRITE|ESP_GATT_CHAR_PROP_BIT_NOTIFY;
static uint16_t s_handle_table[ASM_IDX_END];

static const esp_gatts_attr_db_t gatt_db[ASM_IDX_END] =
{
  [ASM_IDX_SVC] = SERVICE_DEFINITION(),
  [ASM_IDX_GET_PROPERTY_CHAR] = CHARACTERISTIC_DEFINITION(),
  [ASM_IDX_GET_PROPERTY_VAL] = VALUE_DEFINITION(s_get_property_uuid, GET_PROPERTY_MAX_LEN),
  [ASM_IDX_GET_PROPERTY_CFG] = CFG_DEFINITION(),
  [ASM_IDX_SET_PROPERTY_CHAR] = CHARACTERISTIC_DEFINITION(),
  [ASM_IDX_SET_PROPERTY_VAL] = VALUE_DEFINITION(s_set_property_uuid, SET_PROPERTY_MAX_LEN),
  [ASM_IDX_SET_PROPERTY_CFG] = CFG_DEFINITION(),
  [ASM_IDX_COMMAND_CHAR] = CHARACTERISTIC_DEFINITION(),
  [ASM_IDX_COMMAND_VAL] = VALUE_DEFINITION(s_command_uuid, COMMAND_MAX_LEN),
  [ASM_IDX_COMMAND_CFG] = CFG_DEFINITION(),
};

static esp_ble_adv_params_t adv_params = {
    .adv_int_min        = 0x100,
    .adv_int_max        = 0x100,
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

static esp_ble_adv_data_t adv_config = {
    .set_scan_rsp = false,
    .include_txpower = true,
    .min_interval = 0x0006, //slave connection min interval, Time = min_interval * 1.25 msec
    .max_interval = 0x0010, //slave connection max interval, Time = max_interval * 1.25 msec
    .appearance = 0x00,
    .manufacturer_len = 0, //TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data = NULL, //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(s_service_uuid),
    .p_service_uuid = s_service_uuid,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};
static esp_ble_adv_data_t scan_rsp_config = {
    .set_scan_rsp = true,
    .include_name = true,
    .manufacturer_len = sizeof(manufacturer),
    .p_manufacturer_data = manufacturer,
};

static bool init_ble_module() {
  esp_err_t ret;

  ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

  esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  ret = esp_bt_controller_init(&bt_cfg);
  if (ret) {
    ESP_LOGE(TAG, "%s init controller failed: %s", __func__, esp_err_to_name(ret));
    return false;
  }
  ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
  if (ret) {
    ESP_LOGE(TAG, "%s enable controller failed: %s", __func__, esp_err_to_name(ret));
    return false;
  }

  ESP_LOGI(TAG, "%s init bluetooth", __func__);
  ret = esp_bluedroid_init();
  if (ret) {
    ESP_LOGE(TAG, "%s init bluetooth failed: %s", __func__, esp_err_to_name(ret));
    return false;
  }
  ret = esp_bluedroid_enable();
  if (ret) {
    ESP_LOGE(TAG, "%s enable bluetooth failed: %s", __func__, esp_err_to_name(ret));
    return false;
  }
  return true;
}

static int get_attirbute(int handle) {
  for (int index =0; index < ASM_IDX_END; ++index){
    if (s_handle_table[index] == handle){
      return index;
    }
  }
  return ASM_IDX_END;
}

/*
static void print_attribute(int attribute) {
  switch (attribute){
    case ASM_IDX_SVC:
      ESP_LOGI(TAG, "attribute ASM_IDX_SVC");
      break;
    case ASM_IDX_PROPERTY_CHAR:
      ESP_LOGI(TAG, "attribute ASM_IDX_PROPERTY_CHAR");
      break;
    case ASM_IDX_PROPERTY_VAL:
      ESP_LOGI(TAG, "attribute ASM_IDX_PROPERTY_VAL");
      break;
    case ASM_IDX_PROPERTY_CFG:
      ESP_LOGI(TAG, "attribute ASM_IDX_PROPERTY_CFG");
      break;
    case ASM_IDX_COMMAND_CHAR:
      ESP_LOGI(TAG, "attribute ASM_IDX_COMMAND_CHAR");
      break;
    case ASM_IDX_COMMAND_VAL:
      ESP_LOGI(TAG, "attribute ASM_IDX_COMMAND_VAL");
      break;
    case ASM_IDX_COMMAND_CFG:
      ESP_LOGI(TAG, "attribute ASM_IDX_COMMAND_CFG");
      break;
    default:
      ESP_LOGW(TAG, "unknown attribute handle");
  }
}
*/
static void create_device(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param){
  ESP_LOGI(TAG, "ESP_GATTS_REG_EVT called");
      //generate a resolvable random address
      esp_ble_gap_config_local_privacy(true);

      uint8_t const* mac = esp_bt_dev_get_address();
      char device_name[DEVICE_NAME_PREFIX_LEN + 13]; // -mac
      sprintf(device_name, "%s-%x%x%x%x%x%x", s_device_name_prefix, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
      esp_ble_gap_set_device_name(device_name);

      esp_ble_gatts_create_attr_tab(gatt_db, gatts_if, ASM_IDX_END, 0);
}

static void start_service(esp_ble_gatts_cb_param_t *param){
  ESP_LOGI(TAG, "The number handle = %x",param->add_attr_tab.num_handle);
  if (param->create.status == ESP_GATT_OK){
    if(param->add_attr_tab.num_handle == ASM_IDX_END) {
      memcpy(s_handle_table, param->add_attr_tab.handles,
      sizeof(s_handle_table));
      esp_ble_gatts_start_service(s_handle_table[ASM_IDX_SVC]);
    }
    else{
      ESP_LOGE(
        TAG,
        "Create attribute table abnormally, num_handle (%d) doesn't equal to ASM_IDX_END(%d)",
        param->add_attr_tab.num_handle,
        ASM_IDX_END
      );
    }
  }
  else{
    ESP_LOGE(TAG, " Create attribute table failed, error code = %x", param->create.status);
  }
}

void process_read_request(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param){
  ESP_LOGW(TAG, "attempt for direct read with conn_id %d, trans_id %d, handle %d\n", param->read.conn_id, param->read.trans_id, param->read.handle);
  // int attribute = get_attirbute(param->read.handle);
  // //print_attribute(attribute);
  // if (attribute == ASM_IDX_GET_PROPERTY_VAL){
  //   //ESP_LOGI(TAG, "property getter for property: %d", property_id);
  //   esp_gatt_rsp_t rsp;
  //   memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
  //   rsp.attr_value.handle = param->read.handle;
  //   rsp.attr_value.len = 4;
  //   rsp.attr_value.value[0] = 0xde;
  //   rsp.attr_value.value[1] = 0xed;
  //   rsp.attr_value.value[2] = 0xbe;
  //   rsp.attr_value.value[3] = 0xef;
  //   esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id, ESP_GATT_OK, &rsp);
  // }
}

static void process_write_for_cfg(esp_ble_gatts_cb_param_t *param){
  uint16_t descr_value = param->write.value[1]<<8 | param->write.value[0];
      if (descr_value == 0x0001){
        ESP_LOGD(TAG, "notify enabled");
        // uint8_t notify_data[15];
        // for (int i = 0; i < sizeof(notify_data); ++i) {
        //   notify_data[i] = i%0xff;
        // }
        // esp_ble_gatts_send_indicate(
        //   gatts_if, param->write.conn_id, s_handle_table[ASM_IDX_SET_PROPERTY_VAL], sizeof(notify_data), notify_data, false
        // );
      }
      else if (descr_value == 0x0000){
        ESP_LOGD(TAG, "notify/indicate disabled");
      }else{
        ESP_LOGE(TAG, "unknown descr value");
        //esp_log_buffer_hex(TAG, param->write.value, param->write.len);
      }
}

static void get_property_value_from_lua(event_data_t data, int data_size){
  ESP_LOGI(TAG, "get_property_value_from_lua");
  get_value_event_data_t* event_data = (get_value_event_data_t*)data;

  lua_getglobal(s_L, "ble_get_value_callback");
  lua_pushinteger(s_L, event_data->property_id);
  if (lua_pcall(s_L, 1, 1, 0))
  {
      ESP_LOGE(TAG, "ble_get_value_callback pcall failed: %s\n", lua_tostring(s_L, -1));
  }
  double value = lua_tonumber(s_L, -1);
  lua_pop(s_L, 1);
  ESP_LOGI(TAG, "Lua provided value %lf", value);

  esp_ble_gatts_send_indicate(
    event_data->gatts_if, event_data->conn_id, s_handle_table[ASM_IDX_GET_PROPERTY_VAL], sizeof(double), (uint8_t *)&value, false
  );
}

static void process_write_for_get_property_value(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param){
  ESP_LOGI(TAG, "received property getter");
  if (param->write.len != sizeof(uint8_t)){
    ESP_LOGE(TAG, "unexpected set property write data size %d", param->write.len);
  }
  else{
    get_value_event_data_t event_data = {
      .gatts_if = gatts_if,
      .conn_id = param->write.conn_id,
      .property_id = param->write.value[0]
    };
    //event loop is used to be appropriate code executed in main task and thread
    ESP_LOGI(TAG, "rising get_value event, size:%d", sizeof(get_value_event_data_t));
    hugo_raise_event(EVENT_LOOP_TYPE_PRIMARY, s_get_value_event_id, &event_data, sizeof(get_value_event_data_t));
  }
}

static void set_property_value_to_lua(event_data_t data, int data_size){
  ESP_LOGI(TAG, "set_property_value_to_lua");
  set_value_event_data_t* event_data = (set_value_event_data_t*)data;

  lua_getglobal(s_L, "ble_set_value_callback");
  lua_pushinteger(s_L, event_data->property_id);
  lua_pushnumber(s_L, event_data->value);
  if (lua_pcall(s_L, 2, 1, 0))
  {
      ESP_LOGE(TAG, "ble_set_value_callback pcall failed: %s\n", lua_tostring(s_L, -1));
  }
  bool success = lua_toboolean(s_L, -1);
  lua_pop(s_L, 1);
  ESP_LOGI(TAG, "Lua returned value %d", success);

  ESP_LOGI(TAG, "sending indication for %d", s_handle_table[ASM_IDX_SET_PROPERTY_VAL]);
  esp_ble_gatts_send_indicate(
    event_data->gatts_if, event_data->conn_id, s_handle_table[ASM_IDX_SET_PROPERTY_VAL], sizeof(uint8_t), (uint8_t*)&success, false
  );
}

static void process_write_for_set_property_value(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param){
  ESP_LOGD(TAG, "received property setter");
  if (param->write.len != sizeof(uint8_t) + sizeof(double)){
    ESP_LOGE(TAG, "unexpected set property write data size %d", param->write.len);
  }
  else{
    set_value_event_data_t event_data = {
      .gatts_if = gatts_if,
      .conn_id = param->write.conn_id,
      .property_id = param->write.value[0],
      .value = *(double*)(param->write.value + 1)
    };
    ESP_LOGD(TAG, "rising set_value event, size:%d", sizeof(set_value_event_data_t));
    hugo_raise_event(EVENT_LOOP_TYPE_PRIMARY, s_set_value_event_id, &event_data, sizeof(set_value_event_data_t));
  }
}

static void command_to_lua(event_data_t data, int data_size){
  ESP_LOGI(TAG, "command_to_lua");
  command_with_data_t* event_data = (command_with_data_t*)data;

  lua_getglobal(s_L, "ble_command_callback");
  lua_pushinteger(s_L, event_data->command_id);
  lua_pushlstring(s_L, (char const *)event_data->data, DATA_FOR_COMMAND_SIZE);
  if (lua_pcall(s_L, 2, 1, 0))
  {
      ESP_LOGE(TAG, "ble_command_callback pcall failed: %s\n", lua_tostring(s_L, -1));
  }
  bool success = lua_toboolean(s_L, -1);
  lua_pop(s_L, 1);
  ESP_LOGI(TAG, "Lua returned value %d", success);

  ESP_LOGI(TAG, "sending indication for %d", s_handle_table[ASM_IDX_COMMAND_VAL]);
  esp_ble_gatts_send_indicate(
    event_data->gatts_if, event_data->conn_id, s_handle_table[ASM_IDX_COMMAND_VAL], sizeof(uint8_t), (uint8_t*)&success, false
  );
}

static void process_write_for_command(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param){
  ESP_LOGD(TAG, "received command");
  //generic command

  //particular commands
  if (param->write.len == 0){
    ESP_LOGE(TAG, "no command_id received");
    return;
  }

  uint8_t command_id = param->write.value[0];
  uint8_t const * in_data =  param->write.value + 1;
  uint8_t in_data_size = param->write.len - sizeof(uint8_t);
  uint8_t * out_data = NULL;
  size_t out_data_size = 0;
  if (s_command_callback(command_id, in_data, in_data_size, &out_data, &out_data_size)){ //first try to process command in provided callback function(shell)
    esp_ble_gatts_send_indicate(
      gatts_if, param->write.conn_id, s_handle_table[ASM_IDX_COMMAND_VAL], out_data_size, out_data, false
    );
  }
  else{
    if (param->write.len > (sizeof(uint8_t) + sizeof(DATA_FOR_COMMAND_SIZE))){ //command_id + data
      ESP_LOGE(TAG, "unexpected command data size %d", param->write.len);
      return;
    }

    command_with_data_t event_data = {
      .gatts_if = gatts_if,
      .conn_id = param->write.conn_id,
      .command_id = command_id
    };
    memset(event_data.data, 0, DATA_FOR_COMMAND_SIZE);
    memcpy(event_data.data, in_data, in_data_size);

    ESP_LOGD(TAG, "rising set_value event, size:%d", sizeof(command_with_data_t));
    hugo_raise_event(EVENT_LOOP_TYPE_PRIMARY, s_command_event_id, &event_data, sizeof(command_with_data_t));
  }
}

static void process_write_request(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param){
  ESP_LOGD(TAG, "GATT_WRITE_EVT, conn_id %d, trans_id %d, handle %d", param->write.conn_id, param->write.trans_id, param->write.handle);
  if (!param->write.is_prep){
    ESP_LOGD(TAG, "GATT_WRITE_EVT, value len %d, value :", param->write.len);
    //esp_log_buffer_hex(TAG, param->write.value, param->write.len);

    int attr = get_attirbute(param->write.handle);
    //print_attribute(attribute);

    if ((attr == ASM_IDX_GET_PROPERTY_CFG || attr == ASM_IDX_SET_PROPERTY_CFG || attr == ASM_IDX_COMMAND_CFG ) && param->write.len == 2) {
      process_write_for_cfg(param);
    }
    else if (attr == ASM_IDX_GET_PROPERTY_VAL){
       process_write_for_get_property_value(gatts_if, param);
    }
    else if (attr == ASM_IDX_SET_PROPERTY_VAL){
      process_write_for_set_property_value(gatts_if, param);
    }
    else if (attr == ASM_IDX_COMMAND_VAL){
      process_write_for_command(gatts_if, param);
      ESP_LOGD(TAG, "received command");
    }
    else{
      ESP_LOGW(TAG, "unknown attribute");
    }
  }
}

static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
  switch (event) {
    case ESP_GATTS_REG_EVT:
      create_device(gatts_if, param);
      break;
    case ESP_GATTS_START_EVT:
      break;
    case ESP_GATTS_CREAT_ATTR_TAB_EVT:
        start_service(param);
        break;
    case ESP_GATTS_CONNECT_EVT:
      ESP_LOGI(TAG, "ESP_GATTS_CONNECT_EVT");
      /* start security connect with peer device when receive the connect event sent by the master */
      if (s_secure){
        esp_ble_set_encryption(param->connect.remote_bda, ESP_BLE_SEC_ENCRYPT_MITM);
      }
      break;
    case ESP_GATTS_DISCONNECT_EVT:
      ESP_LOGI(TAG, "ESP_GATTS_DISCONNECT_EVT, disconnect reason 0x%x", param->disconnect.reason);
      /* start advertising again when missing the connect */
      esp_ble_gap_start_advertising(&adv_params);
      break;
    case ESP_GATTS_READ_EVT:
      process_read_request(gatts_if, param);
      break;
    case ESP_GATTS_RESPONSE_EVT:
      ESP_LOGD(TAG, "ESP_GATTS_RESPONSE_EVT");
      break;
    case ESP_GATTS_WRITE_EVT:
      process_write_request(gatts_if, param);
      break;
    case ESP_GATTS_MTU_EVT:
      ESP_LOGD(TAG, "ESP_GATTS_MTU_EVT");
      break;
    case ESP_GATTS_CONF_EVT:
      ESP_LOGD(TAG, "ESP_GATTS_CONF_EVT");
      break;
    default:
      ESP_LOGW(TAG, "unhandled GATT event: %d", event);
      break;
  }
}

/*
static char *esp_key_type_to_str(esp_ble_key_type_t key_type)
{
   char *key_str = NULL;
   switch(key_type) {
    case ESP_LE_KEY_NONE:
      key_str = "ESP_LE_KEY_NONE";
      break;
    case ESP_LE_KEY_PENC:
      key_str = "ESP_LE_KEY_PENC";
      break;
    case ESP_LE_KEY_PID:
      key_str = "ESP_LE_KEY_PID";
      break;
    case ESP_LE_KEY_PCSRK:
      key_str = "ESP_LE_KEY_PCSRK";
      break;
    case ESP_LE_KEY_PLK:
      key_str = "ESP_LE_KEY_PLK";
      break;
    case ESP_LE_KEY_LLK:
      key_str = "ESP_LE_KEY_LLK";
      break;
    case ESP_LE_KEY_LENC:
      key_str = "ESP_LE_KEY_LENC";
      break;
    case ESP_LE_KEY_LID:
      key_str = "ESP_LE_KEY_LID";
      break;
    case ESP_LE_KEY_LCSRK:
      key_str = "ESP_LE_KEY_LCSRK";
      break;
    default:
      key_str = "INVALID BLE KEY TYPE";
      break;
   }
   return key_str;
}
*/
/*
static char *esp_auth_req_to_str(esp_ble_auth_req_t auth_req)
{
   char *auth_str = NULL;
   switch(auth_req) {
    case ESP_LE_AUTH_NO_BOND:
      auth_str = "ESP_LE_AUTH_NO_BOND";
      break;
    case ESP_LE_AUTH_BOND:
      auth_str = "ESP_LE_AUTH_BOND";
      break;
    case ESP_LE_AUTH_REQ_MITM:
      auth_str = "ESP_LE_AUTH_REQ_MITM";
      break;
    case ESP_LE_AUTH_REQ_BOND_MITM:
      auth_str = "ESP_LE_AUTH_REQ_BOND_MITM";
      break;
    case ESP_LE_AUTH_REQ_SC_ONLY:
      auth_str = "ESP_LE_AUTH_REQ_SC_ONLY";
      break;
    case ESP_LE_AUTH_REQ_SC_BOND:
      auth_str = "ESP_LE_AUTH_REQ_SC_BOND";
      break;
    case ESP_LE_AUTH_REQ_SC_MITM:
      auth_str = "ESP_LE_AUTH_REQ_SC_MITM";
      break;
    case ESP_LE_AUTH_REQ_SC_MITM_BOND:
      auth_str = "ESP_LE_AUTH_REQ_SC_MITM_BOND";
      break;
    default:
      auth_str = "INVALID BLE AUTH REQ";
      break;
   }
   return auth_str;
}
*/
static void show_bonded_devices(void)
{
    int dev_num = esp_ble_get_bond_device_num();

    esp_ble_bond_dev_t *dev_list = (esp_ble_bond_dev_t *)malloc(sizeof(esp_ble_bond_dev_t) * dev_num);
    esp_ble_get_bond_device_list(&dev_num, dev_list);
    ESP_LOGD(TAG, "Bonded devices number : %d\n", dev_num);

    // ESP_LOGI(TAG, "Bonded devices list : %d\n", dev_num);
    // for (int i = 0; i < dev_num; i++) {
    //     esp_log_buffer_hex(TAG, (void *)dev_list[i].bd_addr, sizeof(esp_bd_addr_t));
    // }

    free(dev_list);
}

static void set_local_privacy(esp_ble_gap_cb_param_t *param){
  if (param->local_privacy_cmpl.status != ESP_BT_STATUS_SUCCESS){
    ESP_LOGE(TAG, "config local privacy failed, error status = %x", param->local_privacy_cmpl.status);
  }

  esp_err_t ret = esp_ble_gap_config_adv_data(&adv_config);
  if (ret){
    ESP_LOGE(TAG, "config adv data failed, error code = %x", ret);
  }else{
    s_adv_config_done |= ADV_CONFIG_FLAG;
  }

  ret = esp_ble_gap_config_adv_data(&scan_rsp_config);
  if (ret){
    ESP_LOGE(TAG, "config adv data failed, error code = %x", ret);
  }else{
    s_adv_config_done |= SCAN_RSP_CONFIG_FLAG;
  }
}

static void ble_auth_cmpl(esp_ble_gap_cb_param_t *param){
  esp_bd_addr_t bd_addr;
  memcpy(bd_addr, param->ble_security.auth_cmpl.bd_addr, sizeof(esp_bd_addr_t));
  ESP_LOGI(TAG, "remote BD_ADDR: %08x%04x",
    (bd_addr[0] << 24) + (bd_addr[1] << 16) + (bd_addr[2] << 8) + bd_addr[3],
    (bd_addr[4] << 8) + bd_addr[5]
  );
  ESP_LOGI(TAG, "address type = %d", param->ble_security.auth_cmpl.addr_type);
  ESP_LOGI(TAG, "pair status = %s",param->ble_security.auth_cmpl.success ? "success" : "fail");
  if(!param->ble_security.auth_cmpl.success) {
      ESP_LOGW(TAG, "fail reason = 0x%x",param->ble_security.auth_cmpl.fail_reason);
  } /*else {
      ESP_LOGI(TAG, "auth mode = %s",esp_auth_req_to_str(param->ble_security.auth_cmpl.auth_mode));
  }*/
  show_bonded_devices();
}

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
  switch (event) {
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
      s_adv_config_done &= (~ADV_CONFIG_FLAG);
      if (s_adv_config_done == 0){
        esp_ble_gap_start_advertising(&adv_params);
      }
      break;
    case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
      s_adv_config_done &= (~SCAN_RSP_CONFIG_FLAG);
      if (s_adv_config_done == 0){
        esp_ble_gap_start_advertising(&adv_params);
      }
      break;
    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
      //advertising start complete event to indicate advertising start successfully or failed
      if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
        ESP_LOGE(TAG, "Advertising start failed\n");
      }
      break;
    case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
      if (param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS) {
          ESP_LOGE(TAG, "Advertising stop failed\n");
      } else {
        ESP_LOGD(TAG, "Stop adv successfully\n");
      }
      break;
    case ESP_GAP_BLE_LOCAL_IR_EVT:                               /* BLE local IR event */
      ESP_LOGD(TAG, "ESP_GAP_BLE_LOCAL_IR_EVT");
      break;
    case ESP_GAP_BLE_LOCAL_ER_EVT:                               /* BLE local ER event */
      ESP_LOGD(TAG, "ESP_GAP_BLE_LOCAL_ER_EVT");
      break;
    case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
      ESP_LOGD(TAG, "ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT");
      break;
    case ESP_GAP_BLE_SET_LOCAL_PRIVACY_COMPLETE_EVT:
        set_local_privacy(param);
        break;
    case ESP_GAP_BLE_KEY_EVT:
        //shows the ble key info share with peer device to the user.
        //ESP_LOGI(TAG, "key type = %s", esp_key_type_to_str(param->ble_security.ble_key.key_type));
        break;
    case ESP_GAP_BLE_AUTH_CMPL_EVT: {
        ble_auth_cmpl(param);
        break;
    }
    default:
      ESP_LOGW(TAG, "unhandled GAP event: %d", event);
      break;
  }
}

static bool register_callbacks(){
  esp_err_t ret;
  ret = esp_ble_gatts_register_callback(gatts_event_handler);
  if (ret){
    ESP_LOGE(TAG, "gatts register error, error code = %x", ret);
    return false;
  }
  ret = esp_ble_gap_register_callback(gap_event_handler);
  if (ret){
    ESP_LOGE(TAG, "gap register error, error code = %x", ret);
    return false;
  }
  ret = esp_ble_gatts_app_register(APP_ID);
  if (ret){
    ESP_LOGE(TAG, "gatts app register error, error code = %x", ret);
    return false;
  }
  return true;
}

static void set_security_params(){
  /* set the security iocap & auth_req & key size & init key response key parameters to the stack*/
  esp_ble_auth_req_t auth_req = ESP_LE_AUTH_REQ_SC_MITM_BOND;     //bonding with peer device after authentication
  esp_ble_io_cap_t iocap = ESP_IO_CAP_NONE;           //set the IO capability to No output No input
  uint8_t key_size = 16;      //the key size should be 7~16 bytes
  uint8_t init_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
  uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
  //set static passkey
  uint32_t passkey = 123456;
  uint8_t auth_option = ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_DISABLE;
  uint8_t oob_support = ESP_BLE_OOB_DISABLE;
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_STATIC_PASSKEY, &passkey, sizeof(uint32_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &iocap, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &key_size, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_ONLY_ACCEPT_SPECIFIED_SEC_AUTH, &auth_option, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_OOB_SUPPORT, &oob_support, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY, &init_key, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key, sizeof(uint8_t));
}

bool hugo_ble_initialize(lua_State* L, char * _device_name_prefix, command_callback_t command_callback){
  s_L = L;
  s_command_callback = command_callback;

  s_get_value_event_id = hugo_get_new_event_id(EVENT_LOOP_TYPE_PRIMARY);
  hugo_add_event_action(EVENT_LOOP_TYPE_PRIMARY, s_get_value_event_id, get_property_value_from_lua);

  s_set_value_event_id = hugo_get_new_event_id(EVENT_LOOP_TYPE_PRIMARY);
  hugo_add_event_action(EVENT_LOOP_TYPE_PRIMARY, s_set_value_event_id, set_property_value_to_lua);

  s_command_event_id = hugo_get_new_event_id(EVENT_LOOP_TYPE_PRIMARY);
  hugo_add_event_action(EVENT_LOOP_TYPE_PRIMARY, s_command_event_id, command_to_lua);


  if (!init_ble_module()) {
    return false;
  }

  int pref_len =  _device_name_prefix != NULL ? strlen(_device_name_prefix) : 0;
  if (pref_len > DEVICE_NAME_PREFIX_LEN - 1) {
    pref_len = DEVICE_NAME_PREFIX_LEN - 1;
  }

  if (pref_len > 0){
    strncpy(s_device_name_prefix,  _device_name_prefix, pref_len);
  }

  if (!register_callbacks()){
    return false;
  }

  s_secure = false;
  if (s_secure)
    set_security_params();
  return true;
}
