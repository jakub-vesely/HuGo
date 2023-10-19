/*
BOARD: hugo_adapter-i2c-pull-ups driving power-block, ambient-block and ble-block
Chip: ATtiny412 or ATtiny1614
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

//#include <avr/wdt.h>
#include <avr/sleep.h>

#include <stdint.h>
#include <Arduino.h>
#include <hugo_defines.h>
#include <tiny_main_base.h>
#include <tiny_main_display.h>
#include <tiny_main_ambient.h>
#include <tiny_main_ble.h>
#include <tiny_main_power.h>
#include <tiny_main_rj12.h>

#define MESH_MAIN_NODE_ID 0x04

#define disp_line_with 11
#define disp_line_count 6
static char disp_lines[disp_line_count][disp_line_with];

char line[disp_line_with];

tiny_common_buffer_t* buffer = tiny_main_base_get_common_buffer();

#define DEC_NR_BUFFER_SIZE 8
static char s_dec_nr_buffer[DEC_NR_BUFFER_SIZE];

static bool heartbeat = false;

char* fill_decimal_number(int32_t number, uint16_t decimal_shift, uint8_t precision = 4){
  s_dec_nr_buffer[DEC_NR_BUFFER_SIZE-1] = '\0';
  char buffer_pos = DEC_NR_BUFFER_SIZE - 1;
  bool negative = number < 0;
  if (negative){
    number = -number; //number is abs now
  }
  bool dot_present = false;
  while (true){
    if (number == 0){
      s_dec_nr_buffer[--buffer_pos] = '0';
      if (decimal_shift == 0){
        break;
      }
    }

    uint8_t digits = 0;
    uint32_t temp = number;
    while (true){
      if (temp == 0){
        break;
      }
      if (decimal_shift == 0){ //remains integer only
        s_dec_nr_buffer[--buffer_pos] = '0' + temp % 10;
      }
      else{
        digits++;
      }
      temp /= 10;
    }

    if (decimal_shift == 0){ //whole number was already processed
      break;
    }

    uint8_t lost_nr = number % 10;
    number /= 10;

    if (digits > precision){
      if (lost_nr > 4){
        number += 1; //rounding
      }
    }
    else if (digits > 0){
       s_dec_nr_buffer[--buffer_pos] = '0' + lost_nr;
       precision--;
    }

    if (--decimal_shift == 0){
      s_dec_nr_buffer[--buffer_pos] = '.';
      dot_present = true;
    }
  }

  if (negative){
    s_dec_nr_buffer[--buffer_pos] = '-';
  }

  //remove redundant decimal zeros
  if (dot_present){
    uint8_t last_pos = DEC_NR_BUFFER_SIZE - 2;
    while (true){
      if (s_dec_nr_buffer[last_pos] == '0'){
        s_dec_nr_buffer[last_pos] = '\0';
      }
      else if (s_dec_nr_buffer[last_pos] == '.'){
        s_dec_nr_buffer[last_pos] = '\0';
        break;
      }
      else{
        break;
      }
      last_pos--;
    }
  }
  return s_dec_nr_buffer + buffer_pos;
}

void add_line(uint8_t* text, uint8_t size){
  for (uint8_t index = 0; index < disp_line_count-1; index++){
     strcpy(disp_lines[index], disp_lines[index+1]);
  }
  uint8_t line_length = min(disp_line_with-1, size);
  memcpy(disp_lines[disp_line_count-1], text, line_length);
  disp_lines[disp_line_count-1][line_length] = '\0';
}

void add_line(tiny_common_buffer_t* p_buffer){
  add_line(p_buffer->data, p_buffer->size);
}

void add_line(char const* text){
  add_line((uint8_t*)text, strlen(text));
}

void lines_to_disp(){
  display.clearDisplay();
  for (uint8_t index = 0; index < disp_line_count; index++){
    display.setCursor(0,index * 8);
    display.print(disp_lines[index]);
  }
}

void send_and_receive_command(char const * command){
  buffer->size = strlen(command);
  memcpy(buffer->data, command, buffer->size);
  buffer->data[buffer->size++] = '\r';
  buffer->data[buffer->size++] = '\n';

  add_line(buffer);
  tiny_main_ble_send_command(I2C_BLOCK_TYPE_ID_BLE);
  delay(100);
  buffer->size = 0;
  tiny_main_ble_get_command_response(I2C_BLOCK_TYPE_ID_BLE);

  add_line(buffer);

  lines_to_disp();
  display.display();
}

void measure_bme(){
  bme.startMeasurement();
  uint8_t timeout = 10;
  while (!bme.isMeasuring()) {
    delay(1);
    if (timeout-- == 0){
      break;
    }
  }

  timeout = 100;
  while (bme.isMeasuring()) {
    delay(1);
    if (timeout-- == 0){
      break;
    }
  }
}

void add_to_common_buffer(char const* text){
  uint8_t append_size = min((COMMON_BUFFER_SIZE - 1), strlen(text));
  strncpy((char*)(buffer->data + buffer->size), text, append_size);
  buffer->size += append_size;
  buffer->data[buffer->size] = '\0';
}

void publish_buffer(bool to_display){
  if (to_display){
    display.println((char*)buffer->data);
  }

  tiny_main_ble_mesh_message.size = min(buffer->size, MAIN_BLE_MESH_BUFFER_SIZE);
  memcpy(tiny_main_ble_mesh_message.data, buffer->data, tiny_main_ble_mesh_message.size);
  tiny_main_ble_send_mesh_data(I2C_BLOCK_TYPE_ID_BLE, MESH_MAIN_NODE_ID);
}

void publish_value(char const * variable, char const* value, char const* unit, bool to_display=true){
  buffer->size = 0;
  buffer->data[0] = '\0';
  add_to_common_buffer(variable);
  add_to_common_buffer("=");
  add_to_common_buffer(value);
  if (strlen(unit) > 0){
    add_to_common_buffer(" ");
    add_to_common_buffer(unit);
  }
  publish_buffer(to_display);
  delay(50);
}

void RTC_init(void)
{
  /* Initialize RTC: */
  while (RTC.STATUS > 0)
  {
    ;                                   /* Wait for all register to be synchronized */
  }
  RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;    /* 32.768kHz Internal Ultra-Low-Power Oscillator (OSCULP32K) */

  RTC.PITINTCTRL = RTC_PI_bm;           /* PIT Interrupt: enabled */

  RTC.PITCTRLA = RTC_PERIOD_CYC32768_gc /* RTC Clock Cycles 16384, resulting in 32.768kHz/32768 = 1Hz */
  | RTC_PITEN_bm;                       /* Enable PIT counter: enabled */
}

ISR(RTC_PIT_vect)
{
  RTC.PITINTFLAGS = RTC_PI_bm;          /* Clear interrupt flag by writing '1' (required) */
}

void setup()
{
  RTC_init();
  line[0] = '\0';
  for (uint8_t index = 0; index < disp_line_count; index++){
    disp_lines[index][0] = '\0';
  }

  tiny_main_base_init();
  delay(10);//it is necessary to wait a while to all blocks and its extensions are is started
  tiny_main_power_init();
  tiny_main_ambient_init();
  tiny_main_display_init();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //wdt_disable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  // send_and_receive_command("AT");
  // delay(1000);
  //send_and_receive_command("AT+NAMEHG-03");
  //delay(1000);
  // send_and_receive_command("AT+ROLE5");
  // delay(1000);
  // send_and_receive_command("AT+NETID1122");
  // delay(1000);
  // send_and_receive_command("AT+MADDR0003");
  // delay(1000);
  // send_and_receive_command("AT+MCLSS0");
  tiny_main_rj12_set_pin_mode(I2C_BLOCK_TYPE_ID_RJ12, Rj12PinId::pin4, Rj12PinMode::input);
  tiny_main_rj12_set_pin_mode(I2C_BLOCK_TYPE_ID_RJ12, Rj12PinId::pin5, Rj12PinMode::input);
}

void loop()
{
  tiny_main_base_set_build_in_led(true);
  delay(20);
  tiny_main_base_set_build_in_led(false);
  display.clearDisplay();
  display.setCursor(0,0);

  charging_state_t charging_state = tiny_main_power_get_charging_state();
  heartbeat = !heartbeat;
  publish_value("heart", heartbeat ? "1" : "0" , "", false);
  publish_value("charg", charging_state.is_charging ? "1" : "0" , "", false);
  publish_value("usb", charging_state.is_usb_connected ? "1" : "0" , "", false);
  display.print(heartbeat ? "* " : "+ ");
  display.print("charg:");
  display.print(charging_state.is_usb_connected ? "1" : "0");
  display.println(charging_state.is_charging ? "1" : "0");

  int32_t voltage_mV = tiny_main_power_get_bat_voltage_mV();
  char* str = fill_decimal_number(voltage_mV, 3, 3);
  publish_value("U", str, "V");

  int32_t current_uA = tiny_main_power_get_bat_current_uA();
  str = fill_decimal_number(current_uA, 3, 3);
  publish_value("I", str, "mA");

  measure_bme();

  // str = fill_decimal_number(bme.getTemperature(), 2, 3);
  // publish_value("t", str, "'C");

  // str = fill_decimal_number(bme.getPressure(), 2, 3);
  // publish_value("P", str, "hPa");

  // str = fill_decimal_number(bme.getHumidity() * 100 / 1024, 2, 3);
  // publish_value("RH", str, "%");

  uint16_t rj12_pin4 =  tiny_main_rj12_get_pin_value_analog(I2C_BLOCK_TYPE_ID_RJ12, Rj12PinId::pin4);
  publish_value("p4", str, "");

  uint16_t rj12_pin5 =  tiny_main_rj12_get_pin_value_analog(I2C_BLOCK_TYPE_ID_RJ12, Rj12PinId::pin5);
  publish_value("p5", str, "");

  tiny_main_base_set_power_save(I2C_ADDRESS_BROADCAST, POWER_SAVE_DEEP);
  display.display();
  delay(500);

  tiny_main_base_set_build_in_led(true);
  delay(100);
  tiny_main_base_set_build_in_led(false);

  for (uint8_t count = 0; count < 120; count++){
    //delay(1000 * 2);
    sleep_cpu();
  }
  //delay(100);
  //wdt_enable(WDT_PERIOD_8KCLK_gc);
  //sleep_cpu();
  //wdt_reset();
  //wdt_disable();
  tiny_main_base_set_power_save(I2C_ADDRESS_BROADCAST, POWER_SAVE_NONE);
}
