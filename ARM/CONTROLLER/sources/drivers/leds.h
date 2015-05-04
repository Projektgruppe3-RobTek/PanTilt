#pragma once
#include "../../headers/tm4c123gh6pm.h"
void setup_leds(void);

#define LED_STATUS_PORT		GPIO_PORTD_DATA_R
#define LED_RGB_PORT			GPIO_PORTF_DATA_R

#define LED_RED 		1<<1
#define LED_YELLOW 		1<<2
#define LED_GREEN 	1<<3
#define LED_STATUS 	1<<6
