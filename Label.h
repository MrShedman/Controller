#pragma once

#include "Arduino.h"
#include "TextGFX.h"
#include "LCD.h"

class Label : public TextGFX
{
public:

	Label()
	{
		setTextSize(2);
	}

	void draw(bool force_draw = false)
	{
		if (force_draw || !equal(&m_old_buf, &m_new_buf))
		{
			draw(&m_old_buf, BLACK, BLACK);

			draw(&m_new_buf, textcolor, textbgcolor);

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

	void draw(const TextBuffer* b, uint16_t textcolor, uint16_t textbgcolor)
	{
		for (uint8_t i = 0; i < b->count; ++i)
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

	bool equal(const TextBuffer* t1, const TextBuffer* t2)
	{
		const uint8_t max_c = max(t1->count, t2->count);

		return (memcmp(t1->text, t2->text, sizeof(uint8_t)*max_c) == 0);
	}

	TextBuffer m_old_buf;
	TextBuffer m_new_buf;
};