#include "uart_stuff.h"
#include "../drivers/UART.h"
#include "../libs/print.h"
#include "controller_task.h"
#include "../drivers/sysctl.h"
#include "sampler_tasks.h"
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
					set_goal1(position);
				//goal1 = 50;
			}
			else
			{
				set_goal2(position);
				//goal2 = 0;
			}
			//vprintf_(uart0_out_string, 200, "%d\n%d\n%d\n",
			//	(int)reset, (int)motor_num, (int)position);
		}
		vTaskDelayUntil(&xLastWakeTime, UART_CONTROL_WAIT_TIME / portTICK_RATE_US );
	}
}

void uart_response(void __attribute__((unused)) *pvParameters)
{	//sends the current location over uart with a certain interval
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	while(1)
	{
		/*//send data for motor1
		INT16S position = get_position1();
		INT16U data = position;
	  data &= ~(1 << 15);
	  data &= ~(1 << 14);
	  data &= ~(1 << 13);
	  data |= 0 << 14;
	  data |= (position < 0) << 13;
	  INT8U first = data >> 8;
	  INT8U last  = data & 0xff;
		uart0_out_char(first);
		uart0_out_char(last);

		//send data for motor2
		position = get_position1();
		data = position;
	  data &= ~(1 << 15);
	  data &= ~(1 << 14);
	  data &= ~(1 << 13);
	  data |= 1 << 14;
	  data |= (position < 0) << 13;
	  first = data >> 8;
	  last  = data & 0xff;
		uart0_out_char(first);
		uart0_out_char(last);
		*/
		vprintf_(uart0_out_string, 200, "%d\n", TIMER1_TAV_R);
		vTaskDelayUntil(&xLastWakeTime, UART_RESPONSE_WAIT_TIME / portTICK_RATE_US );
	}

}
