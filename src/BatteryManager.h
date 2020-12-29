#pragma once

#include "Arduino.h"
#include "RingBuffer.h"
#include "elapsedMillis.h"

class BatteryManager
{
public:

	enum State : uint8_t
	{
		DISCHARGING,
		CHARGING,
		CHARGED
	};

	void update();

	const char* getState() const
	{
		return batteryStateTable[state].name;
	}

	State state;
	float voltage;
	uint8_t percent;
	uint32_t time_remaining; // minutes
	uint32_t last_plugged_in;

private:

	struct Log
	{
		Log() : value(0.0f), time(0.0f) {}

		Log(float value, float time)
			:
			value(value),
			time(time)
		{}

		float value;
		float time;
	};

	const static uint16_t log_entries = 200;
	const uint32_t log_delta = 3000; //milliseconds
	const uint32_t log_min_time = 60; //seconds
	elapsedMillis log_timer;
	RingBuffer<Log, log_entries> log_buffer;

	void predictRemainingTime();

	bool linreg(float *m, float *b) const;

private:

	typedef struct batteryStateTableEntry_s
	{
		State state;
		const char *name;
	}
	batteryStateTableEntry_t;

	const batteryStateTableEntry_t batteryStateTable[3] = {
		{ DISCHARGING,	"DISCHARGING" },
		{ CHARGING,		"CHARGING" },
		{ CHARGED,		"CHARGED" }
	};
};

extern BatteryManager battery;