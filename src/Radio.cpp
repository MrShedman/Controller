#include "Radio.h"
#include "Pins.h"

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <printf.h>

#include "System.h"
#include "Logger.h"

Radio radio;

namespace
{
	volatile bool tx_success = false;
	volatile bool tx_failed = false;
	volatile bool rx_available = false;

	void radio_interrupt()
	{
		radio.interruptHandler();
	}
}

Radio::Radio()
	:
	rf24(RF24_CE_PIN, RF24_CS_PIN),
	m_mode(None),
	m_payload(nullptr),
	m_ackPayload(nullptr),
	m_restart_delay(2000),
	m_timeout(1000),
	m_update_rate(10),
	m_RSSI(0)
{}

void Radio::interruptHandler()
{
	bool tx, fail, rx;
	rf24.whatHappened(tx, fail, rx);

	tx_success = tx;
	tx_failed = fail;
	rx_available = rx;
}

bool Radio::hasConnection()
{
	return ackCounter(m_timeout) > 0;
}

uint16_t Radio::ackCounter(uint32_t dt_ms)
{
	const uint32_t dt = millis() - dt_ms;
	const uint32_t n = m_fifo.size();
	uint16_t counter = 0;

	for (uint8_t i = 0; i < n; ++i)
	{
		if (m_fifo[n] > dt)
		{
			counter++;
		}
		else
		{
			break;
		}
	}

	return counter;
}

void Radio::calculateRSSI()
{
	const uint16_t temp = (uint16_t)ceilf(0.8f * (float)m_update_rate + 6.86f);

	m_RSSI = constrain(ackCounter(1000) * 100 / temp, 0, 100);
}

void Radio::update()
{
	//if (m_mode == Receive) enable(false);

	/*

	if (rx_available)
	{
		if (m_mode == Transmit)
		{
			rf24.read(m_ackPayload->data(), m_ackPayload->size());
		}
		else if (m_mode == Receive)
		{
			rf24.read(m_payload->data(), m_payload->size());
			rf24.writeAckPayload(1, m_ackPayload->data(), m_ackPayload->size());
		}

		m_fifo.push(millis());

		rx_available = false;
	}

	*/

	/*
	if (tx_success)
	{
		m_fifo.push(millis());
	}
	
	calculateRSSI();

	if (!hasConnection())
	{
		m_payload->reset();
		m_ackPayload->reset();

		if (m_restart_timer > m_restart_delay)
		{
			enable(false);
			begin(m_mode);

			m_restart_timer = 0;
		}
	}
	*/
	if (m_mode == Transmit && m_timer > 1e3 / m_update_rate)
	{
		//rf24.write(m_payload->data(), m_payload->size());
		rf24.write(m_payload->data(), m_payload->size());
		m_timer = 0;
	}

	//if (m_mode == Receive) enable(true);
}

void Radio::enable(bool flag)
{
	LOG_INFO("Radio %s", flag ? "enable" : "disable");

	if (flag)
	{
		attachInterrupt(RF24_IRQ_PIN, radio_interrupt, LOW);
	}
	else
	{
		detachInterrupt(RF24_IRQ_PIN);
	}
}

void Radio::begin(Mode mode)
{
	m_mode = mode;

	rf24.begin();
	//rf24.setChannel(125);
	rf24.setPALevel(RF24_PA_MAX);
	//rf24.setAutoAck(false);
	//rf24.enableAckPayload();
	//rf24.enableDynamicPayloads();
	//rf24.setCRCLength(RF24_CRC_16);
	rf24.setDataRate(RF24_250KBPS);
	//rf24.setRetries(4, 1);

	const uint64_t pipeOut = 0xE8E8F0F0E1LL;

	if (m_mode == Transmit)
	{
		rf24.openWritingPipe(pipeOut);// address[0]);
		//rf24.openReadingPipe(1, address[1]);
	}
	else if (m_mode == Receive)
	{
		rf24.openWritingPipe(address[1]);
		rf24.openReadingPipe(1, address[0]);

		rf24.writeAckPayload(1, m_ackPayload->data(), m_ackPayload->size());
	}

	//rf24.startListening();

	//rf24.printDetails();

	m_fifo.reset();

	pinMode(RF24_IRQ_PIN, INPUT);
	//enable(true);
}