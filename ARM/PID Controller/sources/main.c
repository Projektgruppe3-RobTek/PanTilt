#include "os/scheduler.h"
#include "os/systick.h"
#include "../headers/setup.h"
#include "drivers/leds.h"
#include "drivers/buttons.h"
#include "drivers/index_sensors.h"
#include "drivers/UART.h"
#include "tasks/spi_handler.h"
int main(void)
{
	disable_global_int();
	setup_leds();
	setup_systick();
	init_scheduler();
	sys_ringbuf_uchar_init();
	setup_uart0();
	setup_buttons();
	setup_index_sensors();
	enable_global_int();


	add_task(spi_handler);
	start_scheduler();

	return 0;
}
