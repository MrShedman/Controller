#pragma once

#define USE_TEENSY3_OPTIMIZED_CODE

#include <SPI.h>
#include "SD.h"
#include "elapsedMillis.h"

volatile bool card_detect = false;
volatile bool card_interrupt = false;
const uint32_t card_delay = 100;
elapsedMillis card_timer;

const uint8_t SD_CS_PIN = 10;
const uint8_t SD_IRQ_PIN = 26;

void sdcard_detect_interrupt()
{
	if (card_timer > card_delay)
	{
		card_interrupt = true;
		card_timer = 0;
	}
}

void init_sdcard()
{
	pinMode(SD_CS_PIN, OUTPUT);
	digitalWriteFast(SD_CS_PIN, LOW);

	pinMode(SD_IRQ_PIN, INPUT_PULLUP);
	attachInterrupt(SD_IRQ_PIN, sdcard_detect_interrupt, CHANGE);
	delayMicroseconds(card_delay);
	//card_interrupt = true;
  	//openCard();
}

void open_sdcard()
{
	card_detect = !digitalReadFast(SD_IRQ_PIN);

	if (card_interrupt)
	{
		card_interrupt = false;

		if (!card_detect) return;
	}
}