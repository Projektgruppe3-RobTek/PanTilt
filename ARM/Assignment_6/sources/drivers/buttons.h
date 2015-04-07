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
#pragma once

/***************************** Include files ********************************/
#include "../../headers/emp_type.h"
#include <stdbool.h>
#include <stdint.h>
#include "../../headers/tm4c123gh6pm.h"
#include "../os/system_buffers.h"

/*******************************   Defines    *******************************/
#define LONG_PRESS_TIME 2000 	// Long press time in ms.
#define DOUBLE_PRESS_TIME 100 // Double click time in ms.

#define NO_BUTTON_EVENT		0
#define SW1_SINGLE_PRESS 	1
#define SW2_SINGLE_PRESS 	2
#define SW1_DOUBLE_PRESS 	3
#define SW2_DOUBLE_PRESS 	4
#define SW1_LONG_PRESS 		5
#define SW2_LONG_PRESS 		6

#define BUTTON_CHECK_PERIODE 5 //in ms

#define SW1_PIN 		1<<4
#define SW2_PIN			1<<0

/*****************************   Functions   ********************************/

void collect_button_events(void);
RBUF_INDEX_TYPE button_data_available(void);
INT8U button_in_char(void);
void setup_buttons(void);

/****************************** End of module *******************************/
