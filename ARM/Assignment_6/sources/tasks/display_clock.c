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
#include "display_clock.h"
#include "clock.h"
#include "../drivers/lcd0.h"
#include "../../headers/emp_type.h"
#include "../os/scheduler.h"

/*****************************   Functions   ********************************/
void display_clock(void)
/**********************************************
* Input : Pointer to LCD Display, Struct time
* Output : None
* Function : Display time on display, check if any digits
* 					 have changed and display them
**********************************************/
{
	if(check_release(lcd0_available))
	{
		lcd0_lock();
		static time time_last ={99 , 99, 99};
		time time_now = get_clock();
		if(time_last.hour != time_now.hour)	// Update hour if changed.
		{
			lcd0_set_cursor(0,0);
			lcd0_write_char('0' + time_now.hour / 10);
			lcd0_write_char('0' + time_now.hour % 10);
		}

		if(time_last.sec != time_now.sec) 		// Update  if second changed.
		{
			lcd0_set_cursor(2, 0);

			if(time_now.sec % 2 == 0)
				lcd0_write_char(':');
			else
				lcd0_write_char(' ');
		}

		if(time_last.hour != time_now.min) 		// Update min if changed.
		{
			lcd0_set_cursor(3, 0);
			lcd0_write_char('0' + time_now.min / 10);
			lcd0_write_char('0' + time_now.min % 10);
		}

		time_last = time_now;
		lcd0_unlock();
	}
}
/****************************** End of module *******************************/
