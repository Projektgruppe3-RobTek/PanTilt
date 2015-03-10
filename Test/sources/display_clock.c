/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: display_clock.c
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: See header
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150225  ALL		Created file.
* 150226	MS 		Fixed syntax
*****************************************************************************/

/***************************** Include files ********************************/
#include "../headers/display_clock.h"

/*****************************   Functions   ********************************/
void display_clock(lcd *lcd_disp, time *time_s)
/**********************************************
* Input : Pointer to LCD Display, Struct time
* Output : None
* Function : Display time on display, check if any digits
* 					 have changed and display them
**********************************************/
{
	static time time_last = {99, 99, 99};
	
	if(time_last.hour != time_s->hour)	// Update hour if changed.
	{
		lcd_set_cursor(lcd_disp,0,0);
		lcd_write(lcd_disp, '0' + time_s->hour / 10);
		lcd_write(lcd_disp, '0' + time_s->hour % 10);
	}

	if(time_last.sec != time_s->sec) 		// Update  if second changed.
	{
		lcd_set_cursor(lcd_disp, 2, 0);

		if(time_s->sec % 2 == 0)
			lcd_write(lcd_disp, ':');
		else
			lcd_write(lcd_disp, ' ');
	}

	if(time_last.min != time_s->min) 		// Update min if changed.
	{
		lcd_set_cursor(lcd_disp, 3, 0);
		lcd_write(lcd_disp, '0' + time_s->min / 10);
		lcd_write(lcd_disp, '0' + time_s->min % 10);
	}

	time_last = *time_s;
}

/****************************** End of module *******************************/
