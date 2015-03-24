/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: button_events.h
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: Functions for initialize button and getting button presses.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150217  StefanRvo		Created file.
* 150226	MS 					Fixed syntax.
*****************************************************************************/
#ifndef _BUTTON_EVENTS_
	#define _BUTTON_EVENTS_

/***************************** Include files ********************************/
#include "emp_type.h"
#include "GLOBAL_DEFINITIONS.h"
#include <stdbool.h>
#include <stdint.h>
#include "tm4c123gh6pm.h"

/*******************************   Defines    *******************************/
#define LONG_PRESS_TIME 2000 	// Long press time in ms.
#define DOUBLE_PRESS_TIME 100 // Double click time in ms.

/*******************************    Enums     *******************************/
typedef enum {
	NO_EVENT,
	SINGLE_PRESS,
	DOUBLE_PRESS,
	LONG_PRESS,
} event;

typedef enum {
	IDLE,
	FIRST_PUSH,
	SECOND_PUSH,
	LONG_PUSH,
	FIRST_RELEASE,
} button_state;

/*******************************   Structs    *******************************/
typedef struct {
	volatile uint32_t *port;
	uint32_t pin;
	button_state state;
	INT16U timer;
} button;

/*****************************   Functions   ********************************/
bool button_pressed(button *button_s);
event get_button_event(button *button_s);
button button_init(volatile uint32_t *port, uint32_t pin);

/****************************** End of module *******************************/
#endif

