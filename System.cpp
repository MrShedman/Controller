#include "System.h"
#include "Pins.h"
#include "elapsedMillis.h"
#include "RingBuffer.h"

IntervalTimer hapTimer;

bool haptic_is_on = false;

void hapticOff()
{
	digitalWriteFast(HAP_PIN, HIGH);
	hapTimer.end();
	haptic_is_on = false;
}

void hapticOn(uint32_t duration)
{
	if (!haptic_is_on)
	{
		digitalWriteFast(HAP_PIN, LOW);
		hapTimer.begin(hapticOff, duration);
		haptic_is_on = true;
	}
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}