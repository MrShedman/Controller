#pragma once

#define USE_TEENSY3_OPTIMIZED_CODE

#include <SPI.h>
#include "SD.h"
#include "Pins.h"
#include "elapsedMillis.h"

volatile bool card_detect = false;
volatile bool card_interrupt = false;
const uint32_t card_delay = 100;
elapsedMillis card_timer;

void card_detect_interrupt()
{
	if (card_timer > card_delay)
	{
		card_interrupt = true;
		card_timer = 0;
	}
}

void openCard()
{
	card_detect = !digitalReadFast(SD_IRQ_PIN);

	if (card_interrupt)
	{
		card_interrupt = false;

		if (!card_detect) return;
	}
}