/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: GLOBAL_DEFINITIONS.h
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: Definitions which is used for many parts of the application.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150217  StefanRvo   Created file.
* 150226	MS 					Fixed syntax.
*****************************************************************************/
#ifndef _GLOBAL_DEFINITIONS_H
	#define _GLOBAL_DEFINITIONS_H

/***************************** Include files ********************************/
#include <stdint.h>
#include <iso646.h>

/*******************************   Defines    *******************************/
#define FCPU              16000000 // Hz.
#define TIMEOUT_SYSTICK   5        // ms.

#define SW1_PIN 		1<<4
#define SW2_PIN			1<<0
#define LED_RED 		1<<1
#define LED_BLUE 		1<<2
#define LED_GREEN 	1<<3
#define LED_STATUS 	1<<6

#define LCD_D4			1<<4
#define LCD_D5			1<<5
#define LCD_D6			1<<6
#define LCD_D7			1<<7
#define LCD_RS			1<<2
#define LCD_E				1<<3

#define LCD_DATA_PORT 		GPIO_PORTC_DATA_R
#define LCD_RS_E_PORT			GPIO_PORTD_DATA_R
#define LED_STATUS_PORT		GPIO_PORTD_DATA_R

/****************************** End of module *******************************/
#endif
