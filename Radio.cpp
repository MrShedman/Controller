#include "Radio.h"
#include "Pins.h"

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <printf.h>
#include "Sticks.h"

enum _armedState
{
	DISARMED,
	PENDING, 
	ARMED
};

RF24 radio(24, 9);

uint8_t address[][5] = { 0xCC,0xCE,0xCC,0xCE,0xCC , 0xCE,0xCC,0xCE,0xCC,0xCE };

IntervalTimer radioTimer;

Payload payload;

AckPayload ackPayload;
AckPayload prevAckPayload;

void resetData()
{
	payload.throttle = 1000;
	payload.yaw = 1500;
	payload.pitch = 1500;
	payload.roll = 1500;
}

void resetAck()
{
	ackPayload.armed_status = DISARMED;
	ackPayload.bat_voltage = 0.0f;
	ackPayload.yaw = 0.0f;
	ackPayload.pitch = 0.0f;
	ackPayload.roll = 0.0f;

	prevAckPayload = ackPayload;
}

void printJoysticks()
{
	Serial.print("throttle: ");
	Serial.print(payload.throttle);
	Serial.print("  yaw: ");
	Serial.print(payload.yaw);
	Serial.print("  pitch: ");
	Serial.print(payload.pitch);
	Serial.print("  roll: ");
	Serial.println(payload.roll);
}

volatile uint16_t acks_per_second_counter = 0;
uint32_t acks_per_second_timer = 0;
uint32_t acks_per_second = 0;

void radio_interrupt()
{
	bool tx, fail, rx;
	radio.whatHappened(tx, fail, rx);

	if (tx)
	{                                         // Have we successfully transmitted?
											  //Serial.println("Send:OK");
	}
	if (fail)
	{                                         // Have we successfully transmitted?
											  //Serial.println("Send:FAIL");
	}
	if (rx)// || radio.available())
	{                      // Did we receive a message?

						   // If we're the sender, we've received an ack payload
		radio.read(&ackPayload, sizeof(AckPayload));
		acks_per_second_counter++;
		/*Serial.print("Ack: ");
		Serial.print(ackPayload.val);
		Serial.print("  ");
		Serial.print(ackPayload.val1);
		Serial.print("  ");
		Serial.print(ackPayload.val2);
		Serial.print("  ");
		Serial.println(ackPayload.val3);
		*/
	}
}

uint32_t radio_update_time = 0;

void radio_update()
{
	if (acks_per_second_timer == 0 &&
		acks_per_second == 0)
	{
		resetAck();
	}

	if (millis() - acks_per_second_timer > 1000)
	{
		acks_per_second = acks_per_second_counter;
		acks_per_second_counter = 0;
		acks_per_second_timer = millis();
	}

	if ((micros() - radio_update_time) > 10000)
	{
		payload.throttle = s_throttle.value;
		payload.roll = s_roll.value;
		payload.pitch = s_pitch.value;
		payload.yaw = s_yaw.value;

	//	detachInterrupt(RF24_IRQ_PIN);

		radio.startWrite(&payload, sizeof(Payload), 0);

	//	attachInterrupt(RF24_IRQ_PIN, radio_interrupt, LOW);

		radio_update_time = micros();
	}
}

void radio_begin()
{
	resetData();

	printf_begin();

	radio.begin();
	radio.setChannel(125);
	radio.setPALevel(RF24_PA_MAX);
	//radio.enableAckPayload();
	radio.enableDynamicPayloads();
	radio.setDataRate(RF24_250KBPS);
	//radio.setRetries(5, 1);
	radio.setPayloadSize(8);

	radio.openWritingPipe(address[0]);
	//radio.openReadingPipe(1, address[1]);
	//radio.openReadingPipe(1,pipe);
	//radio.startListening();
	radio.printDetails();

	pinMode(RF24_IRQ_PIN, INPUT);
	//attachInterrupt(RF24_IRQ_PIN, radio_interrupt, LOW);

	//radioTimer.begin(radio_update, 1e6 / 100);
}