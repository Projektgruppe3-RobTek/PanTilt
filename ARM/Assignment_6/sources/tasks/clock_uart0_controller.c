#include "clock_uart0_controller.h"
#include "../drivers/UART.h"

static void send_time(void );
static void set_time(INT8U *buffer);

void clock_uart0_controller(void)
{
	static INT8U buffer[7] = {0};
	static INT8U buffer_index = 0;
	while(uart0_data_available())
	{
		INT8U val = uart0_in_char();
		if ((buffer_index == 0 && val == '1') || buffer[0] == '1')
		{
			buffer[buffer_index] = val;
			if(buffer_index == 6)
			{
				set_time(buffer+1);
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
			send_time();
		}
	}
}

static void send_time()
{
	time time_s = get_clock();
	uart0_out_char('2');
	uart0_out_char(time_s.hour / 10 + '0');
	uart0_out_char(time_s.hour % 10 + '0');
	uart0_out_char(time_s.min / 10 + '0');
	uart0_out_char(time_s.min % 10 + '0');
	uart0_out_char(time_s.sec / 10 + '0');
	uart0_out_char(time_s.sec % 10 + '0');
}

static void set_time(INT8U *buffer)
{
	set_clock((buffer[0] - '0') * 10 + (buffer[1] - '0'),
						(buffer[2] - '0') * 10 + (buffer[3] - '0'),
						(buffer[4] - '0') * 10 + (buffer[5] - '0'));
}
