#pragma once

#include <Arduino.h>

void hapticOff();
void hapticOn(uint32_t duration = 70000);

void updateScreenBrightness();

struct Config
{	
	uint8_t radioChannel;
	uint8_t radioPALevel;
	uint8_t radioDataRate;
	uint8_t radioAckPayload;
	uint8_t radioRefreshRate;
	uint8_t radioTimeout;

	uint8_t imuLowPassFilter;
	uint8_t imuGyroFSR;
	uint8_t imuAccelFSR;

	uint8_t beeper;
	uint8_t haptic;

	uint8_t displayBrightness;
};