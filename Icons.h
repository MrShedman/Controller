#pragma once

#include "Arduino.h"

enum atlas_id : uint8_t
{
	bat_charged_ext_pow = 0,
	bat_charg_100,
	bat_charg_80,
	bat_charg_60,
	bat_charg_40,
	bat_charg_20,
	bat_crit,
	bat_100,
	bat_80,
	bat_60,
	bat_40,
	bat_20,
	back,
	home,
	settings,
	sd_card,
	sig_none,
	sig_low,
	sig_med,
	sig_high,
	sig_max
};

class Rect;

Rect getBitmapRect(atlas_id id);

extern const uint16_t bitmap_atlas[];