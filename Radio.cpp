#include "Radio.h"
#include "Pins.h"

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <printf.h>
#include "Sticks.h"
#include "System.h"

Radio radio;

namespace
{
	volatile bool rx_available = false;

	void radio_interrupt()
	{
		radio.interruptHandler();
	}
}

void Radio::interruptHandler()
{
	bool tx, fail, rx;
	rf24.whatHappened(tx, fail, rx);

	if (tx)
	{
		//Serial.println("Send:OK");
	}
	if (fail)
	{
		//Serial.println("Send:FAIL");
	}
	if (rx)// || radio.available())
	{ 
		rx_available = true;
		//rf24.read(m_ackPayload.data, m_ackPayload.size);
		//m_ackCounter.push();
	}
}

bool Radio::hasConnection()
{
	return ackCounter(m_timeout) > 0;
}

uint16_t Radio::ackCounter(uint32_t dt_ms)
{
	const uint32_t* h = m_fifo.head();

	const uint32_t dt = millis() - dt_ms;

	uint16_t counter = 0;

	for (uint8_t i = 0; i < m_fifo.size(); ++i)
	{
		if (*h > dt)
		{
			counter++;

			h = m_fifo.previous(h);
		}
		else
		{
			break;
		}
	}

	return counter;
}

void Radio::update()
{
	if (rx_available)
	{
		rf24.read(m_ackPayload->data(), m_ackPayload->size());
		m_fifo.push(millis());

		rx_available = false;
	}

	if (!hasConnection())
	{
		m_payload->reset();
		m_ackPayload->reset();
	}

	if (m_timer > 1e3 / m_update_rate)
	{
		rf24.startFastWrite(m_payload->data(), m_payload->size(), false);

		m_timer = 0;
	}
}

void Radio::begin()
{
	printf_begin();

	rf24.begin();
	//radio.setChannel(125);
	rf24.setPALevel(RF24_PA_MAX);
	rf24.enableAckPayload();
	//radio.enableDynamicPayloads();
	rf24.setDataRate(RF24_250KBPS);
	rf24.setRetries(4, 1);
	rf24.openWritingPipe(address[0]);
	rf24.openReadingPipe(1, address[1]);

	rf24.printDetails();

	pinMode(RF24_IRQ_PIN, INPUT);
	attachInterrupt(RF24_IRQ_PIN, radio_interrupt, LOW);
}