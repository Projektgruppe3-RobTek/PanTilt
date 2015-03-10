/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: main.c
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: Main loop
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150225	ALL		Created file.
*	150226  MS    Syntax Fixed.
*****************************************************************************/

/***************************** Include files ********************************/
#include <stdint.h>
#include "../headers/setup.h"
#include "../headers/tm4c123gh6pm.h"
#include "../headers/GLOBAL_DEFINITIONS.h"
#include "../headers/button_events.h"
#include "../headers/SWDelay.h"
#include "../headers/lcd.h"
#include "../headers/clock.h"
#include "../headers/display_clock.h"
#include "../headers/systick.h"
#include "../headers/system_buffers.h"
#include "../headers/UART.h"
#include "../headers/clock_uart0_controller.h"
#include "../headers/pe1.h"
#include "../headers/print.h"

/*****************************    Defines    ********************************/
#define STATUS_BLINK_TIME 500 // Blink time for status led in ms.

/*****************************   Functions   ********************************/

void lcd0_write_string(char *string);

lcd lcd_disp;

int main(void)
/**********************************************
* Input : None.
* Output : None.
* Function : Call the setup functions,
*						 enter main loop which runs once per 5 seconds.
**********************************************/
{
	disable_global_int();
	setup_gpio(); 				// Setup SW1, SW2 and LCD pins.
	setup_delay(); 				// Setup timer0 for delay functions.
	setup_systick(); 			// Setup systick timer.
	sys_ringbuf_uchar_init();
	setup_uart0();
	enable_global_int();
	
	// Setup buttons and events.
	button sw1 = button_init(&GPIO_PORTF_DATA_R, SW1_PIN);
	button sw2 = button_init(&GPIO_PORTF_DATA_R, SW2_PIN);
	event sw1_event;
	event sw2_event;

	// Initiate LCD Display.
	lcd_disp = lcd_init_4bit(LCD_RS, LCD_E, (volatile INT32U *)&LCD_RS_E_PORT, LCD_D4,
															 LCD_D5, LCD_D6, LCD_D7, (volatile INT32U *)&LCD_DATA_PORT);
	lcd_begin(&lcd_disp, 2);
	
	static INT32U led_status_timer = STATUS_BLINK_TIME / TIMEOUT_SYSTICK;
	static time time_s ={0, 0, 0};
	while(1)
	{
		while(!ticks);
		ticks--;

		/************ Start Status LED  ************/
		if(led_status_timer > 0)
			led_status_timer--;
		
		if(!led_status_timer)
		{
			LED_STATUS_PORT ^= LED_STATUS;
			led_status_timer = STATUS_BLINK_TIME / TIMEOUT_SYSTICK;
		}
		/************  End Status LED   ************/
		
		// Tasks.
		sw1_event 	= get_button_event(&sw1);
		sw2_event 	= get_button_event(&sw2);
		clock(sw1_event, sw2_event, &time_s);
		clock_uart0_controller(&time_s);
		lcd_set_cursor(&lcd_disp,0,0);
		vprintf_(lcd0_write_string, 200, "%d",(int)pulses);
	}
	return (0);
}
void lcd0_write_string(char *string)
{
	lcd_write_string(&lcd_disp, string);
}

/****************************** End of module *******************************/
