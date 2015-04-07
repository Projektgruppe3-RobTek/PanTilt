/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: clock.c
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: See header.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150225  ALL		Created file.
* 150226	MS 		Fixed syntax.
*****************************************************************************/

/***************************** Include files ********************************/
#include "clock.h"
#include "../drivers/buttons.h"
#include "../os/system_timers.h"
#include "../os/systick.h"

static INT8U clock_timer = 0;
static time clock;


/*****************************   Functions   ********************************/
void run_clock(void)
/**********************************************
* Input : SW1 event, SW2 event.
* Output : Time.
* Function : Count timer normally if buttons not pressed,
* 					 else the time can be changed by the user
* 					 in SET_MIN / SET_HOUR state.
**********************************************/
{
	static clock_state state = NORMAL;
	INT8U event_from_button = NO_BUTTON_EVENT;
	if(button_data_available())
	{
		event_from_button = button_in_char();
	}

	switch(state)
	{
		case NORMAL:
			if(event_from_button == SW1_LONG_PRESS or event_from_button == SW2_LONG_PRESS) state = SET_MIN;
			else if(get_timer_val(clock_timer) <= SECOND_LENGTH / TIMEOUT_SYSTICK)
			{
				increase_timer_val(clock_timer, SECOND_LENGTH / TIMEOUT_SYSTICK);
				time_count_sec(&clock, 1);
			}
			break;

		case SET_MIN:
			if(event_from_button == SW1_SINGLE_PRESS) time_count_min(&clock, 1);
			else if(event_from_button == SW2_SINGLE_PRESS) time_count_min(&clock, -1);
			else if(event_from_button == SW1_LONG_PRESS or event_from_button == SW2_LONG_PRESS)
			{
				state 		 = NORMAL;
				clock.sec = 0;
			}
			else if(event_from_button == SW1_DOUBLE_PRESS or event_from_button == SW2_DOUBLE_PRESS) state = SET_HOUR;
			break;

		case SET_HOUR:
			if(event_from_button == SW1_SINGLE_PRESS) time_count_hour(&clock, 1);
			else if(event_from_button == SW2_SINGLE_PRESS) time_count_hour(&clock, -1);
			else if(event_from_button == SW1_LONG_PRESS or event_from_button == SW2_LONG_PRESS)
			{
				state = NORMAL;
				clock.sec = 0;
			}
			if(event_from_button == SW1_DOUBLE_PRESS or event_from_button == SW2_DOUBLE_PRESS) state = SET_MIN;
			break;

		default:
			break;
	}
}
bool set_clock(INT8U hour_, INT8U min_, INT8U sec_)
{
	if(hour_ < HOURS_PER_DAY && min_ < MINUTES_PER_HOUR && sec_ < SECONDS_PER_MINUTE)
	{
		clock.hour = hour_;
		clock.min = min_;
		clock.sec = sec_;
		return true;
	}
	return false;
}

time get_clock(void)
{
	return clock;
}
void setup_clock(void)
{ //Just accuire a timer for the clock, and set the time to 12:00:00.
	//Start the timer at 2*second lenght
	clock_timer = request_timer();
	start_timer(clock_timer, 2 * SECOND_LENGTH / TIMEOUT_SYSTICK);
	set_clock(12, 0, 0);
}
/****************************** End of module *******************************/
