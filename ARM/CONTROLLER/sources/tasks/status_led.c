/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: leds.c
*
* PROJECT....: ECP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "../../headers/tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "../../headers/emp_type.h"
#include "status_led.h"


/*****************************    Defines    *******************************/
#define PF0		0		// Bit 0

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void status_led_init(void)
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function :
*****************************************************************************/
{
  // Enable the GPIO port that is used for the on-board LED.
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;

  __asm__("NOP");

  GPIO_PORTF_DIR_R |= 0x02;
  GPIO_PORTF_DEN_R |= 0x02;
}


void status_led_task(void __attribute__((unused)) *pvParameters)
{

	while(1)
	{
		// Toggle status led
    GPIO_PORTF_DATA_R ^= 0x02;
		vTaskDelay(200000 / portTICK_RATE_US); // wait 100 ms.
	}
}


/****************************** End Of Module *******************************/
