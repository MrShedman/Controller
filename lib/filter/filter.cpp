#include "filter.h"

void lowPassFilterInit(lowPassFilter_t *filter, float f_cut, Time dT)
{
	filter->RC = 1.0 / (TWO_PI * f_cut);
	filter->dT = dT;
	filter->k = filter->dT.asSeconds() / (filter->RC + filter->dT.asSeconds());
}

float lowPassFilterApply(lowPassFilter_t *filter, float input)
{
	filter->state = filter->state + filter->k * (input - filter->state);
	return filter->state;
}

float lowPassFilterApplyDynamic(lowPassFilter_t *filter, float input)
{
	const Time now = Time::now();
	filter->dT = now - filter->last_update;
	filter->last_update = now;
	filter->k = filter->dT.asSeconds() / (filter->RC + filter->dT.asSeconds());

	filter->state = filter->state + filter->k * (input - filter->state);
	return filter->state;
}

void highPassFilterInit(highPassFilter_t *filter, float f_cut, Time dT)
{
	filter->RC = 1.0 / (TWO_PI * f_cut);
	filter->dT = dT;
	filter->alpha = filter->RC / (filter->RC + filter->dT.asSeconds());
}

float highPassFilterApply(highPassFilter_t *filter, float input)
{
	filter->state = filter->alpha * (filter->state + input - filter->prev_input);
	filter->prev_input = input;
	return filter->state;
}

float highPassFilterApplyDynamic(highPassFilter_t *filter, float input)
{	
	const Time now = Time::now();
	filter->dT = now - filter->last_update;
	filter->last_update = now;
	filter->alpha = filter->RC / (filter->RC + filter->dT.asSeconds());

	filter->state = filter->alpha * (filter->state + input - filter->prev_input);
	filter->prev_input = input;
	return filter->state;
}