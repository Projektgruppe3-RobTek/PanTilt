/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: clock.h
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: Real time clock that can be set using SW1 and SW2.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150225  ALL		Created file.
* 150226	MS 		Fixed syntax.
*****************************************************************************/
#ifndef _CLOCK_H_
	#define _CLOCK_H_

/***************************** Include files ********************************/
#include "button_events.h"
#include "GLOBAL_DEFINITIONS.h"

/*******************************   Defines    *******************************/
#define SECOND_LENGTH 1000 // Length of a second in ms.

/*******************************    Enums     *******************************/
typedef enum {
	NORMAL,
	SET_MIN,
	SET_HOUR,
} clock_state;

/*******************************   Structs    *******************************/
typedef struct {
	INT8U sec;
	INT8U min;
	INT8U hour;
} time;

/*****************************   Functions   ********************************/
void clock_tick(time *time_s);
void clock(event button1_ev, event button2_ev, time *time_s);
void clock_count_min(time *time_s, bool direction);
void clock_count_hour(time *time_s, bool direction);

/****************************** End of module *******************************/
#endif
