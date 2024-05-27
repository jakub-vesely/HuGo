/*
BOARD: hugo_adapter-i2c-pull-ups driving ble-shield, power-block and ambient-block (+ display block optionally)
Chip: ATtiny412 or ATtiny1614
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

//#define WIND_AND_RAIN
//#define USE_DISPLAY

#include <avr/sleep.h>

#include <stdint.h>
#include <Arduino.h>
#include <hugo_defines.h>
#include <tiny_main_base.h>
#include <tiny_main_ambient.h>
#include <tiny_ble_shield.h>
#include <tiny_main_power.h>
#include <tiny_main_rj12.h>


#ifdef USE_DISPLAY
# include <tiny_main_display.h>
#endif

#define MESH_MAIN_NODE_ID 0x04
#define RJ12_RAIN_GAUGE 0x30

// 0,   22,5  45,   67.5, 90, 112.5,  135,  157.5,  180,  202.5 225,  247.5,  270,  292.5,  315,  337.5
// 22k, 33k, 6k8,  8k2,  820,  1k, 680,,   2k2,  1k5,    3k9,  3k3,  15k,  12k,    120k  42k,    68k
static uint16_t wind_vane_values[16] = {143, 301, 259, 637, 607, 946, 831, 888, 707, 789, 417, 471, 100, 111, 84, 198};

uint8_t multiplier = 3;

tiny_common_buffer_t* buffer = tiny_main_base_get_common_buffer();

#define DEC_NR_BUFFER_SIZE 8
static char s_dec_nr_buffer[DEC_NR_BUFFER_SIZE];

static bool heartbeat = false;
BleShield ble_shield;

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
#ifdef USE_DISPLAY
  if (to_display){
    display.println((char*)buffer->data);
  }
#endif

  ble_shield.send_mesh_data(MESH_MAIN_NODE_ID);
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
  //delay(100);
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

  tiny_main_base_init();
  delay(200);//it is necessary to wait a while to all blocks and its extensions are is started
  tiny_main_power_init(false);
  tiny_main_ambient_init();

#ifdef USE_DISPLAY
  tiny_main_display_init();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
#endif

  ble_shield.init();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  tiny_main_rj12_set_pin_mode(I2C_BLOCK_TYPE_ID_RJ12, Rj12PinId::pin5, Rj12PinMode::input);
  tiny_main_rj12_set_pin_mode(I2C_BLOCK_TYPE_ID_RJ12, Rj12PinId::pin4, Rj12PinMode::interrupt_rising);

  tiny_main_rj12_set_pin_mode(RJ12_RAIN_GAUGE, Rj12PinId::pin4, Rj12PinMode::interrupt_rising);
  tiny_main_rj12_pin5_set_oscil_period_ms(RJ12_RAIN_GAUGE, 100);
}

uint8_t get_closest_wind_direction_index(uint16_t measured){
  uint16_t closest_distance = abs(measured - wind_vane_values[0]);
  uint8_t closest_index = 0;
  for (uint8_t index = 1; index < 16; index++){
    uint16_t distance = abs(measured - wind_vane_values[index]);
    if (distance < closest_distance){
      closest_distance = distance;
      closest_index = index;
    }
  }
  return closest_index;
}
void process_power(){
  tiny_main_power_init(true);
  delay(1);
  char* str;
  charging_state_t charging_state = tiny_main_power_get_charging_state();
  publish_value("charg", charging_state.is_charging ? "1" : "0" , "", false);
  publish_value("usb", charging_state.is_usb_connected ? "1" : "0" , "", false);

  int32_t voltage_mV = tiny_main_power_get_bat_voltage_mV();
  str = fill_decimal_number(voltage_mV, 3, 3);
  publish_value("U", str, "V");

  int32_t current_uA = tiny_main_power_get_bat_current_uA();
  str = fill_decimal_number(current_uA, 3, 3);
  publish_value("I", str, "mA");

  // if (voltage_mV < 3400){
  //   multiplier = 10;
  // }
  // else if (voltage_mV < 3500){
  //   multiplier = 6;
  // }
  // else if (voltage_mV < 3700) {
  //   multiplier = 4;
  // }
  // else {
  //   multiplier = 3;
  // }

  tiny_main_power_init(false);
  //tiny_main_base_set_power_save(I2C_BLOCK_TYPE_ID_POWER, POWER_SAVE_DEEP);
}

void process_bme(){
  measure_bme();

  char* str = fill_decimal_number(bme.getTemperature(), 2, 3);
  publish_value("t", str, "'C");

  str = fill_decimal_number(bme.getPressure(), 2, 3);
  publish_value("P", str, "hPa");

  str = fill_decimal_number(bme.getHumidity() * 100 / 1024, 2, 3);
  publish_value("RH", str, "%");
  tiny_main_base_set_power_save(I2C_BLOCK_TYPE_ID_AMBIENT, POWER_SAVE_DEEP);
}

void process_wind(){
  uint16_t rj12_pin5 =  tiny_main_rj12_get_pin_value_analog(I2C_BLOCK_TYPE_ID_RJ12, Rj12PinId::pin5);
  char* str = fill_decimal_number(rj12_pin5, 0, 3);

  uint8_t wind_vane_index = get_closest_wind_direction_index(rj12_pin5);
  uint16_t wind_vane_value = ((uint16_t)wind_vane_index * ((uint16_t)(360 * 10 / (uint16_t)16))) / 10;
  str = fill_decimal_number(wind_vane_value, 0, 0);
  publish_value("wd", str, "deg");

  if (tiny_main_rj12_pin4_get_timestamp_diffs(I2C_BLOCK_TYPE_ID_RJ12)){
    uint32_t* diff1 = (uint32_t*)buffer->data;
    uint32_t* diff2 = (uint32_t*)(buffer->data + 4);

    uint32_t km_h = 0;
    if (*diff2 < 5 * 1000){ //value older than 5 sec doesn make sense
      //1 switch closure/sec = 2.4 km/h = 0.6 m/sec
      km_h = (1000 * 2.4 * 100) / (*diff1); //1000 ms, 100 decimal shift
    }
    str = fill_decimal_number(km_h, 2, 3);
    publish_value("ws", str, "km/h");
  }
}

void process_rain() {
  uint16_t count = tiny_main_rj12_pin4_get_count_and_reset(RJ12_RAIN_GAUGE);

  //1 impulse = 0.2794mm = 279.4 um
  uint16_t amount = 2794 * count;

  char *str = fill_decimal_number(amount, 7, 3);
  publish_value("rain", str, "um");
}

void loop()
{
  tiny_main_base_set_build_in_led(true);
  delay(20);
  tiny_main_base_set_build_in_led(false);

#ifdef USE_DISPLAY
  display.clearDisplay();
  display.setCursor(0,0);
#endif

  //heartbeat = !heartbeat;
  //publish_value("heart", heartbeat ? "1" : "0" , "", false);

  process_power();
  process_bme();

#ifdef WIND_AND_RAIN
  process_wind();
  process_rain();
#endif

#ifdef USE_DISPLAY
  display.display();
  delay(200);
#endif

  //tiny_main_base_set_power_save(I2C_ADDRESS_BROADCAST, POWER_SAVE_DEEP);
  ble_shield.power_save(true);

  tiny_main_base_set_build_in_led(true);
  delay(20);
  tiny_main_base_set_build_in_led(false);
  //delay(200);
#ifdef USE_DISPLAY
  unsigned sec = 3;//10;
#else
  unsigned sec = 60;
#endif
  for (uint8_t count1 = 0; count1 < multiplier; count1++){
    for (uint8_t count2 = 0; count2 < sec; count2++){
      sleep_cpu();
    }
  }

  ble_shield.power_save(false);

  //tiny_main_base_set_power_save(I2C_ADDRESS_BROADCAST, POWER_SAVE_NONE);
}
