#include "Arduino.h"
#include "RadioConfigState.h"
#include "LCD.h"
#include "TextGFX.h"
#include "StateStack.h"
#include "System.h"
#include "Beeper.h"

extern StateStack stateStack;

void RadioConfigState::setup()
{
	checkbox.setShape(Rect(15, 40, 210, 30));
	checkbox.setCallback([](bool) {beeper(BEEPER_SHORT); });
	checkbox.setText("CheckBox!");

	optionbutton.setShape(Rect(15, 80, 210, 30));
	optionbutton.setCallback([](uint8_t) {stateStack.clearScreen(); });
	optionbutton.addSelection("250KBPS");
	optionbutton.addSelection("1MBPS");
	optionbutton.addSelection("2MBPS");
	optionbutton.addSelection("5MBPS");
	optionbutton.addSelection("10MBPS");
	optionbutton.set(0);

	slider_channel.setShape(Rect(15, 120, 210, 30));
	slider_channel.setCallback([](float p) {display.setBrightness(p * 100); });
	slider_channel.setSliderPosition(display.getBrightness() / 100.0f);

	slider_data_rate.setShape(Rect(15, 160, 210, 30));

	textbox.setShape(Rect(15, 200, 210, 30));
	textbox.setCallback([]() {stateStack.clearScreen(); });

	textbox1.setShape(Rect(15, 240, 210, 30));
	textbox1.setCallback([]() {stateStack.clearScreen(); });

	m_container.pack(&checkbox);
	m_container.pack(&optionbutton);
	m_container.pack(&slider_channel);
	m_container.pack(&slider_data_rate);
	m_container.pack(&textbox);
	m_container.pack(&textbox1);

	Serial.println("RadioConfigState succesfully Initialised");
}

void RadioConfigState::handleTouch(const Touch& touch)
{
	m_container.handleTouch(touch);
}

void RadioConfigState::update()
{
	m_container.draw(m_force_redraw);

	m_force_redraw = false;
}