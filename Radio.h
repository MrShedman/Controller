
#pragma once

#include "Arduino.h"

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

enum armedState : uint8_t
{
	DISARMED,
	PENDING,
	ARMED
};

typedef struct armedStateTableEntry_s
{
	uint8_t mode;
	const char *name;
}
armedStateTableEntry_t;

static const armedStateTableEntry_t armedStateTable[] = {
	{ DISARMED,	"DISARMED" },
	{ PENDING,	"PENDING" },
	{ ARMED,	"ARMED" }
};

struct Payload
{
	int16_t throttle;
	int16_t roll;
	int16_t pitch;
	int16_t yaw;
};

struct AckPayload
{
	uint8_t armed_status;
	uint16_t packets_per_second;
	float bat_voltage;
	float roll;
	float pitch;
	float yaw;
};

extern Payload payload;

extern AckPayload ackPayload;

extern uint32_t acks_per_second;

void resetData();

void resetAck();

void printJoysticks();

bool radio_has_connection();

void radio_interrupt();

void radio_update();

void radio_begin();