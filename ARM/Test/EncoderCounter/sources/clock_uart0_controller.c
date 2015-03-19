#include "../headers/clock_uart0_controller.h"
#include "../headers/UART.h"
#include "../headers/print.h"

static void send_time(time *time_s);
static void set_time(INT8U *buffer, time *time_s);

void clock_uart0_controller(time *time_s)
{
	static INT8U buffer[7] = {0};
	static INT8U buffer_index = 0;
	while(uart0_data_avaliable())
	{
		INT8U val = uart0_in_char();
		if ((buffer_index == 0 && val == '1') || buffer[0] == '1')
		{
			buffer[buffer_index] = val;
			if(buffer_index == 6)
			{
				set_time(buffer+1, time_s);
				for(INT8U i = 0; i < 7; i++)
				{
					buffer[i] = 0;
				}
				buffer_index = 0;
			}
			else buffer_index++;
		}
		else if (val == '2')
		{
			send_time(time_s);
		}
	}
}

static void send_time(time *time_s)
{
	
	uart0_out_char('2');
	uart0_out_char(time_s->hour / 10 + '0');
	uart0_out_char(time_s->hour % 10 + '0');
	uart0_out_char(time_s->min / 10 + '0');
	uart0_out_char(time_s->min % 10 + '0');
	uart0_out_char(time_s->sec / 10 + '0');
	uart0_out_char(time_s->sec % 10 + '0');
}

static void set_time(INT8U *buffer, time *time_s)
{
	if( !(
			(buffer[0] - '0') * 10 + (buffer[1] - '0') > 23 ||
			(buffer[2] - '0') * 10 + (buffer[3] - '0') > 59 ||
			(buffer[4] - '0') * 10 + (buffer[5] - '0') > 59 ))
	{
		time_s->hour = (buffer[0] - '0') * 10 + (buffer[1] - '0');
		time_s->min = (buffer[2] - '0') * 10 + (buffer[3] - '0');
		time_s->sec = (buffer[4] - '0') * 10 + (buffer[5] - '0');
	}
}
