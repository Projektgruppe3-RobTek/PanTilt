#include "uart_stuff.h"
#include "../drivers/UART.h"
#include "../libs/print.h"
#include "controller_task.h"
#include "../drivers/sysctl.h"
#include "sampler_tasks.h"
#include "../drivers/leds.h"

#define MIN_0 -212
#define MIN_0_STOP -120
#define MAX_0 230
#define MAX_0_STOP 140
#define MIN_1 -1068
#define MIN_1_STOP (-270 + 270)
#define MAX_1 1067
#define MAX_1_STOP (270 + 270)
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
				if(position <= MAX_0_STOP && position >= MIN_0_STOP)
					set_goal1(position);
				else if(position > MAX_0_STOP)
					set_goal1(MAX_0_STOP);
				else
					set_goal1(MIN_0_STOP);
			}
			else
			{
				if(position <= MAX_1_STOP && position >= MIN_1_STOP)
					set_goal2(position);
				else if(position > MAX_1_STOP)
					set_goal2(MAX_1_STOP);
				else
					set_goal2(MIN_1_STOP);

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
		INT16S position = get_position2();
		INT16U position_abs = abs(position);
		INT16U data = position_abs;
	  data &= ~(1 << 15);
	  data &= ~(1 << 14);
	  data &= ~(1 << 13);
	  data |= 0 << 14;
	  data |= (position < 0) << 13;
	  INT8U first = data >> 8;
	  INT8U last  = data & 0xff;


		//send data for motor2
		INT16S position2 = get_position1();
		INT16U position_abs2 = abs(position2);
		INT16U data2 = position_abs2;
	  data2 &= ~(1 << 15);
	  data2 &= ~(1 << 14);
	  data2 &= ~(1 << 13);
	  data2 |= 1 << 14;
	  data2 |= (position2 < 0) << 13;
	  INT8U first2 = data2 >> 8;
	  INT8U last2  = data2 & 0xff;

		uart0_out_char(first);
		uart0_out_char(last2);
		uart0_out_char(first2);
		uart0_out_char(last);

		vTaskDelayUntil(&xLastWakeTime, UART_RESPONSE_WAIT_TIME / portTICK_RATE_US );
	}

}
