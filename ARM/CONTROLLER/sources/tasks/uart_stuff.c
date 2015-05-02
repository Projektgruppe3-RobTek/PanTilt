#include "uart_stuff.h"
#include "../drivers/UART.h"
#include "../libs/print.h"
#include "controller_task.h"
#include "../drivers/sysctl.h"
void uart_control(void __attribute__((unused)) *pvParameters)
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	while(1)
	{
		if(uart0_data_available())
		{
			INT16U indata = uart0_in_char() << 8;
			indata += uart0_in_char();
			bool reset = indata & (1 << 15);
			bool motor_num = indata & (1 << 14);
			INT16S position = (indata << 2);
			position /= 4;
			if(reset)
			{
				do_reset();
			}
			if(motor_num == 0)
			{
				if(position < 100 && position > -100)
				goal1 = position;
				//goal1 = 50;
			}
			else
			{
				goal2 = position;
				//goal2 = 0;
			}
			vprintf_(uart0_out_string, 200, "%d\n%d\n%d\n",
				(int)reset, (int)motor_num, (int)position);
		}
		vTaskDelayUntil(&xLastWakeTime, UART_TASK_FREQ / portTICK_RATE_US );
	}
}
