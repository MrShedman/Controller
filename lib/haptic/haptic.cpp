#include "haptic.h"

IntervalTimer hapTimer;

const uint8_t HAP_PIN = 33;

bool haptic_is_on = false;

void init_haptic()
{
	digitalWriteFast(HAP_PIN, HIGH);
	pinMode(HAP_PIN, OUTPUT);
}

void haptic_off()
{
	digitalWriteFast(HAP_PIN, HIGH);
	hapTimer.end();
	haptic_is_on = false;
}

void haptic_on(uint32_t duration)
{
	if (!haptic_is_on)
	{
		digitalWriteFast(HAP_PIN, LOW);
		hapTimer.begin(haptic_off, duration);
		haptic_is_on = true;
	}
}