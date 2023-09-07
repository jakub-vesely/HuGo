#include <Wire.h>
#include "tiny_main_base.h"
#include "hugo_defines.h"
#include <pocketBME280.h>

pocketBME280 bme;

void tiny_main_ambient_init(){
    uint8_t bme280_address = tiny_main_base_get_ext_module_address(I2C_BLOCK_TYPE_ID_METEO);
    bme.setAddress(bme280_address);
    bme.begin();
}
