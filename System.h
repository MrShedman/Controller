#pragma once

#include "Arduino.h"

void hapticOff();
void hapticOn(uint32_t duration = 70000);

void update_bat_voltage();