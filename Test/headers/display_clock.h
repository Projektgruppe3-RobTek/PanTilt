/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: display_clock.h
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: Displays time on the LCD Display, but only the digits that
* 						 have changed since.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150225  ALL		Created file.
* 150226	MS 		Fixed syntax
*****************************************************************************/
#ifndef _DISPLAY_CLOCK_H_
	#define _DISPLAY_CLOCK_H_

/***************************** Include files ********************************/
#include "clock.h"
#include "lcd.h"
#include "emp_type.h"

/*****************************   Functions   ********************************/
void display_clock(lcd *lcd_disp, time *time_s);

/****************************** End of module *******************************/
#endif
