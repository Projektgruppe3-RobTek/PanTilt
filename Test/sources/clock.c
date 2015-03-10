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
#include "../headers/clock.h"

/*****************************   Functions   ********************************/
void clock_tick(time *time_s)
/**********************************************
* Input : Struct time.
* Output : None.
* Function : Count timer.
**********************************************/
{
	if(++time_s->sec >= 60)
	{
		time_s->sec = 0;
		if(++time_s->min >= 60)
		{
			time_s->min = 0;
			if(++time_s->hour >= 24)
				time_s->hour = 0;
		}
	}
}

void clock_count_min(time *time_s, bool direction)
/**********************************************
* Input : Struct time, direction of count.
* Output : None.
* Function : Increment time_s->min if direction == true.
* 					 Decrement time_s->min if direction == false.
**********************************************/
{
	if(direction)
		if(time_s->min >= 59)
			 time_s->min = 0;
		else
			 time_s->min++;
	else
	{
		if(time_s->min == 0)
			 time_s->min = 59;
		else
			 time_s->min--;
	}
}

void clock_count_hour(time *time_s, bool direction)
/**********************************************
* Input : Struct time, direction of count.
* Output : None.
* Function : Increment time_s->hour if direction == true.
* 					 Decrement time_s->hour if direction == false.
**********************************************/
{
	if(direction)
		if(time_s->hour >= 23)
			 time_s->hour = 0;
		else
			 time_s->hour++;
	else
	{
		if(time_s->hour == 0)
			 time_s->hour = 23;
		else
			 time_s->hour--;
	}
}

void clock(event button1_ev, event button2_ev, time *time_s)
/**********************************************
* Input : SW1 event, SW2 event.
* Output : Time.
* Function : Count timer normally if buttons not pressed,
* 					 else the time can be changed by the user
* 					 in SET_MIN / SET_HOUR state.
**********************************************/
{
	static INT32U timer = SECOND_LENGTH / TIMEOUT_SYSTICK;
	static clock_state state = NORMAL;
	
	switch(state)
	{
		case NORMAL:
			if(timer > 0)
				 timer--;
				
			if(button1_ev == LONG_PRESS or button2_ev == LONG_PRESS) state = SET_MIN;
			else if(timer == 0)
			{
				timer = SECOND_LENGTH / TIMEOUT_SYSTICK;
				clock_tick(time_s);
			}
			break;
		
		case SET_MIN:
			if(button1_ev == SINGLE_PRESS) clock_count_min(time_s, 1);
			if(button2_ev == SINGLE_PRESS) clock_count_min(time_s, 0);
			if(button1_ev == LONG_PRESS or button2_ev == LONG_PRESS)
			{
				state 		 = NORMAL;
				time_s->sec = 0;
			}
			if(button1_ev == DOUBLE_PRESS or button2_ev == DOUBLE_PRESS) state = SET_HOUR;
			break;
		
		case SET_HOUR:
			if(button1_ev == SINGLE_PRESS) clock_count_hour(time_s, 1);
			if(button2_ev == SINGLE_PRESS) clock_count_hour(time_s, 0);
			if(button1_ev == LONG_PRESS or button2_ev == LONG_PRESS)
			{
				state = NORMAL;
				time_s->sec = 0;
			}
			if(button1_ev == DOUBLE_PRESS or button2_ev == DOUBLE_PRESS) state = SET_MIN;
			break;
		
		default:
			break;
	}
}

/****************************** End of module *******************************/
