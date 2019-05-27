#pragma once

#include "time.h"

struct lowPassFilter_t 
{
	float state;
	float k;
	float RC;
	Time dT;
	Time last_update;
};

struct highPassFilter_t 
{
	float state;
	float prev_input;
	float alpha;
	float RC;
	Time dT;
	Time last_update;
};

void lowPassFilterInit(lowPassFilter_t *filter, float f_cut, Time dT);
float lowPassFilterApply(lowPassFilter_t *filter, float input);
float lowPassFilterApplyDynamic(lowPassFilter_t *filter, float input);

void highPassFilterInit(highPassFilter_t *filter, float f_cut, Time dT);
float highPassFilterApply(highPassFilter_t *filter, float input);
float highPassFilterApplyDynamic(highPassFilter_t *filter, float input);