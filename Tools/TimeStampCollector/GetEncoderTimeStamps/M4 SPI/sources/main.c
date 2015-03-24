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
	setup_ssi3();
	setup_ssi0();
	enable_global_int();
	//SSI2 is MSB, SSI3 is LSB
	INT16U last_tmp1 = 0;
	INT16U last_tmp2 = 0;
	INT8U counter = 0;
	while (1)
	{
		counter++;
		ssi3_out_16bit( (INT16U)counter ); //Request packages
		ssi0_out_16bit( (INT16U)counter ); //Request packages
		delay_microseconds(10);
		while(ssi3_data_avaliable() && ssi0_data_avaliable())
		{
			INT16U temp1 = ssi0_in_16bit();
			INT16U temp2 = ssi3_in_16bit();
			if(temp1 != 0 || temp2 != 0)
			{
				uart0_out_char((INT8U) ((temp1 >> 8) & 0xFF) );
				uart0_out_char((INT8U) (temp1 & 0xFF) );
				uart0_out_char((INT8U) ((temp2 >> 8) & 0xFF) );
				uart0_out_char((INT8U) (temp2 & 0xFF) );

				last_tmp1 = temp1;
				last_tmp2 = temp2;
			}
		}
	}
	return (0);
}

/****************************** End of module *******************************/
