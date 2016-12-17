#pragma once

#include "Arduino.h"

void hapticOff();
void hapticOn(uint32_t duration = 70000);

enum batteryState : uint8_t
{
	DISCHARGING,
	CHARGING,
	CHARGED
};

typedef struct batteryStateTableEntry_s
{
	uint8_t mode;
	const char *name;
}
batteryStateTableEntry_t;

static batteryStateTableEntry_t batteryStateTable[] = {
	{ DISCHARGING,	"DISCHARGING"	},
	{ CHARGING,		"CHARGING"		},
	{ CHARGED,		"CHARGED"		}
};

struct Battery
{
	batteryState state;
	float voltage;
	uint8_t percent;
};

extern Battery battery;

void update_battery();

float mapf(float x, float in_min, float in_max, float out_min, float out_max);