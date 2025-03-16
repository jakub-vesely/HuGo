#include <tiny_ble_shield.h>
#include <tiny_main_base.h>
#include <tiny_main_display.h>
#include <string.h>

#define MESH_MAIN_NODE_ID 0x04
#define COMMUNICATION_TIMEOUT 1000

BleShield ble_shield;
static tiny_common_buffer_t* p_common_buffer = tiny_main_base_get_common_buffer();

//void read_data(){
//  display.println((char*)p_common_buffer->data);
//}

void send_at_command(char const* command){
  tiny_main_string_to_buffer(command);
  ble_shield.command_send_and_receive(COMMUNICATION_TIMEOUT);
}

void setup(){
  tiny_main_base_init();
  tiny_main_display_init();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  ble_shield.init(9600);
  send_at_command("AT+NAMEHG-09");
  //send_at_command("AT+ROLE5");
 // send_at_command("AT+MCLSS0");
  send_at_command("AT+NETIDC000");
  send_at_command("AT+MADDR0009");
}

void loop(){
  tiny_main_base_set_build_in_led_a(true);
  delay(20);
  tiny_main_base_set_build_in_led_a(false);

  tiny_main_string_to_buffer("AT");


  ble_shield.command_send_and_receive(COMMUNICATION_TIMEOUT);

  p_common_buffer->data[p_common_buffer->size] = '\0';
  display.println((char *)p_common_buffer->data);
  display.display();
  delay(1000);
}
