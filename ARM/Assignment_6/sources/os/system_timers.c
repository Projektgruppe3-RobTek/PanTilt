#include "system_timers.h"


static INT16U timers[MAX_TIMERS];
static bool allocated_timers[MAX_TIMERS];

void stop_timer(INT8U id)
{
	if(id < MAX_TIMERS)
	timers[id] = 0;
}
void count_timer(INT8U id)
{
	if(id < MAX_TIMERS && timers[id])
		timers[id]--;
}

INT16U get_timer_val(INT8U id)
{
	if(id < MAX_TIMERS)
		return timers[id];
	else
		return MAX_TIMER_VAL;
}

void increase_timer_val(INT8U id, INT16U val)
{
	INT16U tmp_timer_val = timers[id];
	if(id < MAX_TIMERS)
	{
		timers[id] += val;
		if (timers[id] < tmp_timer_val) //Detect overflow
			timers[id] = MAX_TIMER_VAL;
	}
}

void start_timer(INT8U id, INT16U timer_val)
{
	if(id < MAX_TIMERS)
		timers[id] = timer_val;
}

bool is_timer_finished(INT8U id)
{
	if(id < MAX_TIMERS)
		return !timers[id];
	else
		return false;
}

INT8U request_timer(void)
{
	INT8U returnid = 0xFF;
	for(INT8U i = 0; i < MAX_TIMERS && returnid == 0xFF; i++)
	{
		if(!allocated_timers[i])
		{
			allocated_timers[i] = true;
			returnid = i;
		}
	}
	return returnid;
}
void release_timer(INT8U id)
{
	if(id < MAX_TIMERS)
		allocated_timers[id] = false;
}
