/***************************** Include files ********************************/
#include <stdint.h>
#include "../headers/setup.h"
#include "../headers/tm4c123gh6pm.h"
#include "../headers/GLOBAL_DEFINITIONS.h"
#include "../headers/button_events.h"
#include "../headers/SWDelay.h"
#include "../headers/lcd.h"
#include "../headers/systick.h"
#include "../headers/system_buffers.h"
#include "../headers/UART.h"
#include "../headers/SSI.h"
#include "../headers/leds.h"

/*****************************    Defines    ********************************/
#define STATUS_BLINK_TIME 500 // Blink time for status led in ms.

/*****************************   Functions   ********************************/
int main(void)
{
	disable_global_int();
	setup_gpio(); 				// Setup SW1, SW2 and LCD pins.
	setup_delay(); 				// Setup timer0 for delay functions.
	setup_systick(); 			// Setup systick timer.
	sys_ringbuf_uchar_init();
	setup_uart0();
	setup_ssi0();
	setup_ssi3();
	enable_global_int();
	
	// Setup buttons and events.
	//button sw1 = button_init(&GPIO_PORTF_DATA_R, SW1_PIN);
	//button sw2 = button_init(&GPIO_PORTF_DATA_R, SW2_PIN);
	
	//event sw1_event;
	//event sw2_event;
	
	// Initiate LCD Display.
	lcd lcd_disp = lcd_init_4bit(	LCD_RS, LCD_E, (volatile INT32U *)&LCD_RS_E_PORT, LCD_D4,
					LCD_D5, LCD_D6, LCD_D7, (volatile INT32U *)&LCD_DATA_PORT);
	lcd_begin(&lcd_disp, 2);
	
	static INT32U led_status_timer = STATUS_BLINK_TIME / TIMEOUT_SYSTICK;
	
	while (1)
	{
		while (!ticks);
		ticks--;	
		/************ Start Status LED  ************/
	
		if (led_status_timer > 0)
			led_status_timer--;

		if (!led_status_timer)
		{
			LED_STATUS_PORT ^= LED_STATUS;
			led_status_timer = STATUS_BLINK_TIME / TIMEOUT_SYSTICK;
		}
	
		/************  End Status LED   ************/
		
		// Tasks.
		//sw1_event 	= get_button_event(&sw1);
		//sw2_event 	= get_button_event(&sw2);
		
		set_leds(0);
		
		if (uart0_data_avaliable() >= 2){
			ssi3_out_16bit((uart0_in_char() << 8) + uart0_in_char());
		}
		if (ssi3_data_avaliable() >= 2){
			INT16U temp = ssi3_in_16bit();
			//uart0_out_char((INT8U)((temp >> 8) & 0xFF));
			//uart0_out_char((INT8U)(temp & 0xFF));
			temp = ssi3_in_16bit();
			uart0_out_char((INT8U)((temp >> 8) & 0xFF));
			uart0_out_char((INT8U)(temp & 0xFF));
		}
		/*
		if (uart0_data_avaliable() >= 2)
		{
			uint8_t temp = uart0_in_char();
			if (temp < 0x10){
				set_leds(2);
				ssi0_out_16bit((temp << 8) + uart0_in_char());
			}
			else{
				set_leds(4);
				ssi3_out_16bit((temp << 8) + uart0_in_char());
			}
		}
		
		if (ssi0_data_avaliable())
		{
			set_leds(4);
			
			uart0_out_char(0x00);
			
			INT16U temp = ssi0_in_16bit();
			uart0_out_char((INT8U)((temp >> 8) & 0xFF));
			uart0_out_char((INT8U)(temp & 0xFF));
			temp = ssi0_in_16bit();
			uart0_out_char((INT8U)((temp >> 8) & 0xFF));
			uart0_out_char((INT8U)(temp & 0xFF));
		}
		
		if (ssi3_data_avaliable() >= 2)
		{
			set_leds(4);
			
			//uart0_out_char(0x10);
			
			INT16U temp = ssi3_in_16bit();
			uart0_out_char((INT8U)((temp >> 8) & 0xFF));
			uart0_out_char((INT8U)(temp & 0xFF));
			temp = ssi3_in_16bit();
			uart0_out_char((INT8U)((temp >> 8) & 0xFF));
			uart0_out_char((INT8U)(temp & 0xFF));
		}
		*/
	}
	return (0);
}

/****************************** End of module *******************************/
