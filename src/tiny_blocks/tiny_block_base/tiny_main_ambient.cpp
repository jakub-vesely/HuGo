#include <Wire.h>
#include <pocketBME280.h>
#include "tiny_main_base.h"
#include "hugo_defines.h"

static pocketBME280 s_bme;
static bool s_available = false;

void tiny_main_ambient_init(){
    uint8_t bme280_address = tiny_main_base_get_ext_module_address(I2C_BLOCK_TYPE_ID_AMBIENT);
    s_available = bme280_address != 0;
    if (s_available){
        s_bme.setAddress(bme280_address);
        s_bme.begin();
    }
}

pocketBME280& tiny_main_ambient_bme(){
    return s_bme;
}

bool tiny_main_ambient_is_available(){
    return s_available;
}
