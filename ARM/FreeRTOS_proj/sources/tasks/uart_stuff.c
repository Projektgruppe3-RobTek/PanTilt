#include "uart_stuff.h"
#include "../drivers/UART.h"
#include "controller_task.h"
void uart_control(void __attribute__((unused)) *pvParameters)
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	while(1)
	{
		if(uart0_data_available())
		{
			INT8U indata = (INT8S)uart0_in_char();
			goal2 = indata;
		}
		vTaskDelayUntil(&xLastWakeTime, TICK_RATE / UART_TASK_FREQ );
	}
}
