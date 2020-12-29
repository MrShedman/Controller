#pragma once

#include <Arduino.h>
#include "TextGFX.h"
#include "LCD.h"

class Label : public TextGFX
{
public:

	Label()
	{
		setTextSize(2);
		clear(&m_old_buf);
		clear(&m_new_buf);
	}

	void draw(bool force_draw = false)
	{
		uint8_t start = 0;

		if (force_draw || !equal(&m_old_buf, &m_new_buf, &start))
		{
			draw(&m_old_buf, start, BLACK, BLACK);

			draw(&m_new_buf, start, textcolor, textbgcolor);

			copy(&m_old_buf, &m_new_buf);
		}

		m_new_buf.count = 0;
	}

protected:

	virtual void drawChar(uint8_t c) override
	{
		append(&m_new_buf, c, cursor);
	}

private:

	const static uint8_t m_size = 32;

	struct TextBuffer
	{
		uint8_t count;
		uint8_t text[m_size];
		Point cursor[m_size];
	};

	void clear(TextBuffer* b)
	{
		memset(b->text, 0, sizeof(uint8_t)*m_size);
		memset(b->cursor, 0, sizeof(Point)*m_size);
		b->count = 0;
	}

	void append(TextBuffer* b, uint8_t c, Point p)
	{
		b->text[b->count] = c;
		b->cursor[b->count] = p;

		b->count++;

		if (b->count >= m_size)
		{
			Serial.println("Buffer overun in TextBuffer");
			b->count = m_size - 1;
		}
	}

	void draw(const TextBuffer* b, uint8_t start, uint16_t textcolor, uint16_t textbgcolor)
	{
		for (uint8_t i = start; i < b->count; ++i)
		{
			TextGFX::drawChar(b->cursor[i].x, b->cursor[i].y, b->text[i], textcolor, textbgcolor, textsize);
		}
	}

	void copy(TextBuffer* dest, TextBuffer* src)
	{
		memcpy(dest->text, src->text, sizeof(uint8_t)*src->count);
		memcpy(dest->cursor, src->cursor, sizeof(Point)*src->count);
		dest->count = src->count;
	}

	bool equal(const TextBuffer* t1, const TextBuffer* t2, uint8_t* start)
	{
		const uint8_t max_c = max(t1->count, t2->count);

		for (uint8_t i = 0; i < max_c; ++i)
		{
			if (t1->text[i] != t2->text[i])
			{
				*start = i;
				return false;
			}
		}

		return true;
	}

	TextBuffer m_old_buf;
	TextBuffer m_new_buf;
};