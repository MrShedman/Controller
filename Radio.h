
#pragma once

#include "Arduino.h"

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include "elapsedMillis.h"
#include "RingBuffer.h"

class Radio
{
public:

	struct Payload
	{
		virtual void* data() = 0;
		virtual uint32_t size() = 0;
		virtual void reset() = 0;
	};

	enum Type
	{
		Normal,
		Ack
	};
	
	Radio()
		:
		rf24(24, 9),
		m_payload(nullptr),
		m_ackPayload(nullptr),
		m_timeout(1000),
		m_update_rate(50)
	{}

	void begin();

	bool hasConnection();

	uint16_t ackCounter(uint32_t dt_ms);

	void update();

	void interruptHandler();

	void setPayload(Payload* payload, Type type)
	{
		if (type == Normal)
		{
			m_payload = payload;
		}
		else if (type == Ack)
		{
			m_ackPayload = payload;
		}
	}

private:

	RF24 rf24;

	const uint8_t address[2][5] = { {0xCC,0xCE,0xCC,0xCE,0xCC} ,{ 0xCE,0xCC,0xCE,0xCC,0xCE } };

	Payload* m_payload;
	Payload* m_ackPayload;

	uint32_t m_timeout;
	uint32_t m_update_rate;
	elapsedMillis m_timer;

	RingBuffer<uint32_t, 128> m_fifo;
};

extern Radio radio;