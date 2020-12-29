
#include "Arduino.h"
#include "Numpad.h"
#include "LCD.h"
#include "TextGFX.h"
#include "WString.h"

#include <functional>

Numpad numpad;

namespace
{
	const uint8_t num_chars = 16;
	bool has_decimal = false;

	void process_button_press(const char* message)
	{
		String temp = numpad.getText();

		if (message[0] == '<')
		{
			if (temp.length() > 0)
			{
				if (temp.endsWith('.'))
				{
					has_decimal = false;
				}

				temp.remove(temp.length() - 1u);
			}
		}
		else if (temp.length() >= num_chars)
		{
			return;
		}
		else if (message[0] == '.')
		{
			if (!has_decimal)
			{
				temp.append(message[0]);
				has_decimal = true;
			}
		}
		else
		{
			temp.append(message[0]);
		}

		numpad.setText(temp);
	}

	const char* button_texts[13] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "<", "Ok" };

	void callback_0() { process_button_press(button_texts[0]); }
	void callback_1() { process_button_press(button_texts[1]); }
	void callback_2() { process_button_press(button_texts[2]); }
	void callback_3() { process_button_press(button_texts[3]); }
	void callback_4() { process_button_press(button_texts[4]); }
	void callback_5() { process_button_press(button_texts[5]); }
	void callback_6() { process_button_press(button_texts[6]); }
	void callback_7() { process_button_press(button_texts[7]); }
	void callback_8() { process_button_press(button_texts[8]); }
	void callback_9() { process_button_press(button_texts[9]); }
	void callback_decimal() { process_button_press(button_texts[10]); }
	void callback_delete() { process_button_press(button_texts[11]); }
	void callback_enter() 
	{ 
		numpad.submit();
	}

	void(*func_ptr[13])(void) = {	callback_0, callback_1, callback_2,	callback_3, callback_4, 
									callback_5,	callback_6, callback_7, callback_8,	callback_9, 
									callback_decimal, callback_delete, callback_enter };
}

Numpad::Numpad()
{	
	m_open = false;
	m_text.reserve(num_chars);
	m_text = "";
	has_decimal = false;

	button_numbers[0].setShape(Rect(px, py + 3 * (h+dy), 2*w + dx, h));
	button_numbers[0].setCallback(func_ptr[0]);
	button_numbers[0].setText(button_texts[0]);
	m_container.pack(&button_numbers[0]);

	for (uint ix = 0; ix < 3; ++ix)
	{
		for (uint iy = 0; iy < 3; ++iy)
		{
			const uint8_t index = 1 + ix + iy * 3;
			const uint16_t x = px + ix * w + ix * dx;
			const uint16_t y = py + 2 * (h + dy) - iy * h - iy * dy;

			button_numbers[index].setShape(Rect(x, y, w, h));

			button_numbers[index].setCallback(func_ptr[index]);

			//auto func = std::bind(process_button_press, button_texts[index]);

			//button_numbers[index].setCallback(func);
			
			
			
			button_numbers[index].setText(button_texts[index]);

			m_container.pack(&button_numbers[index]);
		}
	}

	button_decimal.setShape(Rect(px + 2 * (dx + w), py + 3 * (h + dy), w, h));
	button_decimal.setCallback(func_ptr[10]);
	button_decimal.setText(button_texts[10]);

	m_container.pack(&button_decimal);

	button_delete.setShape(Rect(px + 3 * (dx + w), py, w, h));
	button_delete.setCallback(func_ptr[11]);
	button_delete.setText(button_texts[11]);
	button_delete.enableMultiPress(true, 100);

	m_container.pack(&button_delete);

	button_enter.setShape(Rect(px + 3 * (dx + w), py + h + dy, w, h * 3 + dy * 2));
	button_enter.setCallback(func_ptr[12]);
	button_enter.setText(button_texts[12]);

	m_container.pack(&button_enter);
}

void Numpad::setText(const String& text)
{
	m_text = text;

	has_decimal = (m_text.indexOf('.') != -1);

	m_should_draw = true;
}

bool Numpad::handleTouch(const Touch& t)
{
	m_container.handleTouch(t);

	return true;
}

void Numpad::draw(bool force_draw)
{
	m_container.draw(force_draw);

	if (m_should_draw || force_draw)
	{
		display.fillRect(Rect(px, py - h, 3 * dx + 4 * w, h), BLACK, false);
		textgfx.setCursor(px + 4 * w + 3 * dx - 12, py - dy - 16, TextGFX::Right);
		textgfx.setTextSize(2);
		textgfx.print(m_text);
		textgfx.setAlignment(TextGFX::Left);

		m_should_draw = false;
	}
}