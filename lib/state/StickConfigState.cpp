#include "Arduino.h"
#include "StickConfigState.h"
#include "LCD.h"
#include "TextGFX.h"
#include "StateStack.h"
#include "Beeper.h"
#include "Numpad.h"
#include "BatteryManager.h"
#include "Radio.h"
#include "Payloads.h"

extern StateStack stateStack;

void StickConfigState::setup()
{
	m_update_rate = 30;

	Serial.println("StickConfigState succesfully Initialised");
}

void StickConfigState::handleTouch(const Touch& touch)
{

}

void StickConfigState::update()
{
	if (m_timer > 1e3 / m_update_rate)
	{
		int16_t x = 5;
		int16_t y = 35;
		int16_t dy = 17;
		
		label0.setCursor(x, y);
		label0.print("Bat voltage: ");
		label0.print(ackPayload.bat_voltage);
		label0.print("V");
		label0.draw(m_force_redraw);

		label1.setCursor(x, y += dy);
		label1.print("MOSFET Temp: ");
		label1.print(ackPayload.temp_mos_avg);
		label1.print("C");
		label1.draw(m_force_redraw);

		label2.setCursor(x, y += dy);
		label2.print("Mot Current: ");
		label2.print(ackPayload.current_motor);
		label2.print("A");
		label2.draw(m_force_redraw);

		//label3.setCursor(x, y += dy);
		//label3.print("RPM: ");
		//label3.print(ackPayload.rpm);
		//label3.draw(m_force_redraw);

		label4.setCursor(x, y += dy);
		label4.print("Tacho: ");
		label4.print(ackPayload.tachometer_abs);
		label4.draw(m_force_redraw);
		
		m_timer = 0;
	}
	
	m_force_redraw = false;
}