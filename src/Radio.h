
#pragma once

#include <Arduino.h>

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include "elapsedMillis.h"
#include "circular_buffer.h"

class Radio
{
public:

	struct Payload
	{
		enum Type
		{
			Normal,
			Ack
		};

		virtual void* data() = 0;
		virtual uint8_t size() = 0;
		virtual void reset() = 0;
	};

	enum Mode
	{
		Transmit,
		Receive,
		None
	};
	
	Radio();

	void begin(Mode mode);

	bool hasConnection();

	uint16_t ackCounter(uint32_t dt_ms);

	uint8_t getRSSI() const
	{
		return m_RSSI;
	}

	void update();

	void interruptHandler();

	void setPayload(Payload* payload, Payload::Type type)
	{
		if (type == Payload::Normal)
		{
			m_payload = payload;
		}
		else if (type == Payload::Ack)
		{
			m_ackPayload = payload;
		}
	}

	void enable(bool flag);

private:

	void calculateRSSI();

	RF24 rf24;

	Mode m_mode;

	const uint8_t address[2][5] = { {0xCC,0xCE,0xCC,0xCE,0xCC} ,{ 0xCE,0xCC,0xCE,0xCC,0xCE } };

	Payload* m_payload;
	Payload* m_ackPayload;

	elapsedMillis m_timer;

	uint32_t m_restart_delay;
	elapsedMillis m_restart_timer;

	uint32_t m_timeout;
	uint32_t m_update_rate;
	const static uint32_t m_max_update_rate = 128;
	
	uint8_t m_RSSI;

	CircularBuffer<uint32_t, m_max_update_rate> m_fifo;
};

extern Radio radio;