#pragma once

#include <pocketBME280.h>

void tiny_main_ambient_init();
pocketBME280&  tiny_main_ambient_bme();
bool tiny_main_ambient_is_available();
