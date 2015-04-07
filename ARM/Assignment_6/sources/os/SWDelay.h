/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: SWDelay.h
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: A set of delays used to control the LCD Display.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150224  StefanRvo		Created file.
* 150226	MS 					Fixed syntax.
*****************************************************************************/
#pragma once

/***************************** Include files ********************************/
#include "../../headers/emp_type.h"
#include "../../headers/tm4c123gh6pm.h"

/*******************************   Defines    *******************************/
#define FCPU 			16000000
#define US_CYCLES FCPU / 1000000
#define MS_CYCLES FCPU / 1000

/*****************************   Functions   ********************************/
void setup_delay(void); // Setup the timer0 to be used in the delay functions.
void delay_cycles(INT32U delay);
void delay_microseconds(INT32U delayus);
void delay_milliseconds(INT32U delayms);

/****************************** End of module *******************************/
