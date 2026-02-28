#include <tiny_ble_shield.h>
#include <tiny_main_base.h>
#include <tiny_main_display.h>
#include <string.h>
#include <hugo_gpio.h>
#include <hugo_utils.h>

#define MESH_MAIN_NODE_ID 0x04
#define COMMUNICATION_TIMEOUT 1000

BleShield ble_shield;
static tiny_common_buffer_t* p_common_buffer = tiny_main_base_get_common_buffer();

//#define MESH_TARGET

//void read_data(){
//  display.println((char*)p_common_buffer->data);
//}

void send_at_command(char const* command) {
  tiny_main_string_to_buffer(command);
  ble_shield.command_send_and_receive(COMMUNICATION_TIMEOUT);
}

void setup() {
  tiny_main_base_init();

  tiny_main_display_init();
  delay(100);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //ble_shield.init(9600);
  //send_at_command("AT+BAUD8"); //115200
  ble_shield.init(115200);

  //send_at_command("AT+NAMEHG-10");
  //send_at_command("AT+ROLE5");
  //send_at_command("AT+MCLSS0");
  //send_at_command("AT+NETIDC000");
  //send_at_command("AT+MADDR0010");
  
}

void loop() {
#ifdef MESH_TARGET
  uint16_t sender = ble_shield.get_mesh_message(50);
  uint8_t length = 0;
  uint8_t flag_byte = 0;
  if (sender != 0) {
    length = p_common_buffer->size;
    flag_byte = p_common_buffer->data[0];
    if (ble_shield.has_message_flag_byte()) {
      tiny_main_base_shine_red(true);
    }
    // if (ble_shield.is_acq_expected()){
    //   ble_shield.send_response_flag((uint8_t)sender);
    // }
    tiny_main_base_shine_red(false);

    display.clearDisplay();
    display.setCursor(0, 0);
    sprintf((char *)p_common_buffer->data,"%u,%d,%d,%d", sender, (flag_byte & 0b10000000) != 0, (flag_byte & 0b01000000) !=0, flag_byte & 0b00001111);
    display.println((char *)p_common_buffer->data);
    display.display();
  }

#else
  ble_shield.power_save(false);
  tiny_main_base_shine_green(true);
  uint8_t data[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a };
  memcpy(p_common_buffer->data, data, sizeof(data));
  p_common_buffer->size = sizeof(data);
  bool ret_val = ble_shield.send_mesh_data(0x09, true);
  if (!ret_val) {
    tiny_main_base_shine_red(true);
    delay(20);
    tiny_main_base_shine_red(false);
  }
  sleep_for_500ms();  
  ble_shield.power_save(true);
  tiny_main_base_shine_green(false);
#endif

  //send_at_command("AT");
  //p_common_buffer->data[p_common_buffer->size] = '\0';

  //display.println("X");
  //display.println((char *)p_common_buffer->data);
  //display.display();

#ifdef MESH_TARGET
  //delay(10);
#else
  sleep_for_s(10);
#endif
}
