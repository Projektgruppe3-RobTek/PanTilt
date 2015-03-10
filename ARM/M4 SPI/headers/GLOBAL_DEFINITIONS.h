#ifndef _GLOBAL_DEFINITIONS_H
#define _GLOBAL_DEFINITIONS_H

#include <stdint.h>
#include <iso646.h>

#define FCPU                 			16000000 	// Hz.
#define TIMEOUT_SYSTICK      			500        	 // ms.

#define LED_RED 	0x02
#define LED_BLUE 	0x04
#define LED_GREEN	0x08
#define SW1_PIN 	0x10
#define SW2_PIN 	0x01

#define LCD_D4			1<<4
#define LCD_D5			1<<5
#define LCD_D6			1<<6
#define LCD_D7			1<<7
#define LCD_RS			1<<2
#define LCD_E			1<<3

#define LCD_DATA_PORT 		GPIO_PORTC_DATA_R
#define LCD_RS_E_PORT		GPIO_PORTD_DATA_R
#define LED_STATUS_PORT		GPIO_PORTD_DATA_R
#define LED_STATUS		(1 << 6)

#endif
