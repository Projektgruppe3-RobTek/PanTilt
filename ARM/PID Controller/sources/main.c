#include "os/scheduler.h"
#include "os/systick.h"
#include "../headers/setup.h"
#include "drivers/leds.h"
#include "drivers/buttons.h"
int main(void)
{
	disable_global_int();
	setup_leds();
	setup_systick();
	init_scheduler();
	sys_ringbuf_uchar_init();
	setup_uart0();
	setup_buttons();
	enable_global_int();



	start_scheduler();

	return 0;
}
