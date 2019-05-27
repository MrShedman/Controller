#include "State.h"
#include "LCD.h"

extern LCD display;

void State::onSwitch()
{
	m_force_redraw = true;
	display.fillRect(Rect(0, 30, 240, 250), BLACK);
}