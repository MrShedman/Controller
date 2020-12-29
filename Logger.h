#pragma once

#include "Arduino.h"

//#define LOGGING

#ifdef LOGGING
#define LOG(arg)  \
        Serial.print(millis());     \
        Serial.print(": ");    \
        Serial.print(__PRETTY_FUNCTION__); \
        Serial.print(' ');      \
        Serial.println(__LINE__);     \
        arg
#else
#define LOG(arg) arg
#endif