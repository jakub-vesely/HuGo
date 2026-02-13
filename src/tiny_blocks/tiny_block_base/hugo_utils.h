#pragma once

#include <avr/sleep.h>

void RTC_init(RTC_PERIOD_t period);
void sleep_for_s(uint16_t timeout_s);
void sleep_for_500ms();
void sleep_for_250ms();
void sleep_for_125ms();
