#pragma once

#include "Arduino.h"

void hapticOff();
void hapticOn(uint32_t duration = 70000);

float mapf(float x, float in_min, float in_max, float out_min, float out_max);