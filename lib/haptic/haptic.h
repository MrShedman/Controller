#pragma once

#include <Arduino.h>

void init_haptic();
void haptic_off();
void haptic_on(uint32_t duration = 70000);