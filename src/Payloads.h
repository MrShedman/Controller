#pragma once

#include "Radio.h"

enum armedState : uint8_t
{
	DISARMED,
	PENDING,
	ARMED
};

typedef struct armedStateTableEntry_s
{
	armedState mode;
	const char *name;
}
armedStateTableEntry_t;

static const armedStateTableEntry_t armedStateTable[] = {
	{ DISARMED,	"DISARMED" },
	{ PENDING,	"PENDING" },
	{ ARMED,	"ARMED" }
};

struct TimePayload : public Radio::Payload
{
	void* data() override
	{
		return &hours;
	}

	uint8_t size() override
	{
		return 12;
	}

	void reset() override
	{
		hours = 0;
		minutes = 0;
		seconds = 0;
	}

	uint32_t hours;
	uint32_t minutes;
	uint32_t seconds;
};

struct QuadPayload : public Radio::Payload
{
	void* data() override
	{
		return &throttle;
	}

	uint8_t size() override
	{
		return 8;
	}

	void reset() override
	{
		throttle = 1000;
		yaw = 1500;
		pitch = 1500;
		roll = 1500;
	}

	int16_t throttle;
	int16_t roll;
	int16_t pitch;
	int16_t yaw;
};

struct QuadAckPayload : public Radio::Payload
{
	void* data() override
	{
		return &d0;
	}

	uint8_t size() override
	{
		return 8;
	}

	void reset() override
	{
		d0 = 0;
		d1 = 0;
		d2 = 0;
		d3 = 0;
	}

	int16_t d0;
	int16_t d1;
	int16_t d2;
	int16_t d3;
};

extern QuadPayload payload;
extern QuadAckPayload ackPayload;