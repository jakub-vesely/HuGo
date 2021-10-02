/*
 * Copyright (c) 2021 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */
#include <dirent.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_rom_md5.h>
#include <esp_log.h>
#include <event_loop.h>
#include <lua_partition.h>

#define TAG "SHELL"
#define FILE_PATH_LEN 32
#define SUSPEND_FILE_PATH LUA_PARTITION_PATH "/suspend.lua"
#define MD5_SIZE 16
#define OUT_BUFFER_SIZE 48
enum {
    //128 ids for user commands
  SHELL_VERSION               = 0x80, //provided s_version
  SHELL_STOP_PROGRAM          = 0x81,  //Lua program performance will be stopped
  SHELL_START_PROGRAM         = 0x82, //Lua program will be started
  SHELL_GET_NEXT_FILE_INFO    = 0x83, //returns name of next file name in /Lua dir
  SHELL_REMOVE_FILE           = 0x84, //remore required file from /Lua dir
  SHELL_HANDLE_FILE           = 0x85, //prepare file for writing/checksumming
  SHELL_GET_FILE_CHECKSUM     = 0x86, //return content md5 checksum of handled file
  SHELL_APPEND                = 0x87, //add required content to the handeled file
};

static int s_program_start_event_id = -1;

char const s_version[1] = "1";
uint8_t s_out_buffer[OUT_BUFFER_SIZE];
char s_file_path[FILE_PATH_LEN] = "";
uint8_t s_path_prefix_len = sizeof(LUA_PARTITION_PATH);
DIR *s_dir = NULL;

static void restart_callback(event_data_t data, int data_size){
  ESP_LOGI(TAG, "start_program_callback");
  vTaskDelay(100 / portTICK_RATE_MS); //wait a while to be returned value through ble
  esp_restart();
}

void hugo_shell_init(){
  s_program_start_event_id = hugo_get_new_event_id(EVENT_LOOP_TYPE_PRIMARY);
  hugo_add_event_action(EVENT_LOOP_TYPE_PRIMARY, s_program_start_event_id, restart_callback);

  strncpy(s_file_path, LUA_PARTITION_PATH, FILE_PATH_LEN);
  s_file_path[sizeof(LUA_PARTITION_PATH)-1] = '/';
  s_file_path[sizeof(LUA_PARTITION_PATH)] = '\0';
}

bool fill_md5_checksum(){
  md5_context_t context;
  ESP_LOGI(TAG, "file:%s", s_file_path);
  FILE* file = fopen(s_file_path, "rb");
  if (file == NULL){
    return false;
  }

  esp_rom_md5_init(&context);
  int c;
  while((c = fgetc(file)) != EOF) {
    esp_rom_md5_update(&context, (uint8_t*)&c, 1);
  }

  esp_rom_md5_final(s_out_buffer, &context);
  esp_log_buffer_hex(TAG, s_out_buffer, 16);

  fclose(file);
  return true;
}

static void set_file_path(char const * file_name){
  strncpy(s_file_path + s_path_prefix_len, file_name, FILE_PATH_LEN - s_path_prefix_len);
}

static bool fill_next_file_info(){
  if (s_dir == NULL){
    s_dir = opendir(LUA_PARTITION_PATH);
  }
  if (s_dir == NULL){
    ESP_LOGE(TAG, "opendir failed");
    return false;
  }
  struct dirent *entry = readdir(s_dir);
  if (entry == NULL){
    closedir(s_dir);
    s_dir = NULL;
    return false; //dir reading is over
  }

  set_file_path(entry->d_name);
  fill_md5_checksum();
  strncpy((char*)(s_out_buffer + 16), entry->d_name, OUT_BUFFER_SIZE - 16);
  return true;
}

static bool remove_file(char const *file_name){
  set_file_path(file_name);
  ESP_LOGI(TAG, "removing file:%s", s_file_path);
  return (remove(s_file_path) == 0);
}

bool hugo_shell_is_program_suspended(){
  FILE* file = fopen(SUSPEND_FILE_PATH, "r");
  if (file != NULL){
    fclose(file);
    return true;
  }
  return false;
}

static void reboot(){
    ESP_LOGI(TAG, "rebooting...");
  //program will be started in the main task context to be returned value first
  uint8_t *event_data = NULL;
  hugo_raise_event(EVENT_LOOP_TYPE_PRIMARY, s_program_start_event_id, &event_data, 0);

}

static bool stop_program(){
  FILE* file = fopen(SUSPEND_FILE_PATH, "w");
  if (file == NULL){
    return false;
  }
  fclose(file);
  reboot();

  return true;
}

static void start_program(){
  remove(SUSPEND_FILE_PATH);
  reboot();
}

static bool append(uint8_t const* in_data, size_t in_data_size){
  FILE* file = fopen(s_file_path, "ab");
  if (file == NULL){
    ESP_LOGI(TAG, "file was not created");
    return false;
  }
  for (int index=0; index < in_data_size; ++index){
    fputc(in_data[index], file);
  }
  fclose(file);
  ESP_LOGD(TAG, "appended to %s", s_file_path);
  return true;
}

static bool open_file(){
  FILE* file = fopen(s_file_path, "wb");
  if (file == NULL){
    ESP_LOGE(TAG, "file was not created");
    return false;
  }
  ESP_LOGI(TAG, "handling file %s", s_file_path);
  return true;
}

bool hugo_shell_process_command(
  uint8_t command_id, uint8_t const * in_data, size_t in_data_size, uint8_t ** out_data, size_t *out_data_size
){
    //by default is returnded false
    s_out_buffer[0] = 0;
    *out_data = s_out_buffer;
    *out_data_size = 1;

    ESP_LOGD(TAG, "called hugo_shell_process_command");
    bool handled = true;
    switch (command_id) {
        case SHELL_VERSION:
          *out_data = (uint8_t *)s_version;
          *out_data_size = sizeof(s_version);
        break;
        case SHELL_STOP_PROGRAM:
          if (stop_program()){
            s_out_buffer[0] = 1; //return true
          }
        break;
        case SHELL_START_PROGRAM:
          start_program();
          s_out_buffer[0] = 1; //return true
        break;
        case SHELL_GET_NEXT_FILE_INFO:
          if (fill_next_file_info()){
            *out_data = s_out_buffer;
            *out_data_size = MD5_SIZE + strlen((char const*)s_out_buffer + MD5_SIZE);
          }
        break;
        case SHELL_REMOVE_FILE:
          if (remove_file((char const *)in_data)){
            s_out_buffer[0] = 1; //return true
          }
        break;
        case SHELL_HANDLE_FILE:
          set_file_path((char const *)in_data);
          if (open_file()){
            s_out_buffer[0] = 1; //return true
          }
        break;
        case SHELL_GET_FILE_CHECKSUM:
          if (fill_md5_checksum()){
            *out_data = s_out_buffer;
            *out_data_size = MD5_SIZE;
          }
        break;
        case SHELL_APPEND:
          if (append(in_data, in_data_size)){
            s_out_buffer[0] = 1; //return true
          }
        break;
        default:
          handled = false;
        break;
    }
    return handled;
}
