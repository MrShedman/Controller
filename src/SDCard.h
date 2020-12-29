
#pragma once

#define USE_TEENSY3_OPTIMIZED_CODE

#include <SPI.h>
#include "SD.h"
#include "Pins.h"
#include "elapsedMillis.h"
#include "Radio.h"

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

	return;

	//radio.enable(false);
	digitalWriteFast(RF24_CS_PIN, HIGH);

	uint32_t t = millis();

	if (!SD.begin(SD_CS_PIN))
	{
		Serial.println("Card begin failed");
		return;
	}

	Serial.print("Init time: ");
	Serial.print(millis()-t);
	Serial.println("ms");

	if (SD.exists("bench.dat"))
	{
		Serial.println("found bench.dat");
	}

	digitalWriteFast(RF24_CS_PIN, LOW);
	//radio.enable(true);
}