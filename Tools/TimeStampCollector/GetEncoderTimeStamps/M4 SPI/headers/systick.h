/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: ISR.h
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: Interrupt functions.
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150212  MS    			Syntax Fixed.
*	150217  StefanRvo 	Changed to only use systick interrupt.
*****************************************************************************/
#ifndef _ISR_H
  #define _ISR_H

/*****************************    Defines    ********************************/
#define SYSTICK_RELOAD_VALUE 		FCPU / 1000 * TIMEOUT_SYSTICK - 1

#if (SYSTICK_RELOAD_VALUE > 0xFFFFFF)
	#error "SYSTICK_RELOAD_VALUE is too high"
#endif

/***************************** Include files ********************************/
#include "tm4c123gh6pm.h"
#include "GLOBAL_DEFINITIONS.h"
#include "emp_type.h"

extern volatile INT32U ticks; // System tick.

/*****************************   Functions   ********************************/
void systick_timer_isr(void);
void setup_systick(void);

/****************************** End of module *******************************/
#endif
