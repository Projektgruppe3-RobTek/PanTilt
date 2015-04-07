#include "os/scheduler.h"
#include "os/systick.h"
#include "../headers/setup.h"
#include "drivers/leds.h"
#include "os/system_buffers.h"
#include "drivers/UART.h"
#include "drivers/keyboard.h"
#include "drivers/buttons.h"
#include "tasks/clock.h"
#include "drivers/lcd0.h"
#include "tasks/display_clock.h"
#include "os/SWDelay.h"
#include "tasks/clock_uart0_controller.h"
#include "tasks/clock_keyboard_controller.h"
#include "drivers/rotary_encoder0.h"
#include "libs/print.h"
#include "tasks/alarm_clock.h"
#include "../headers/assert.h"
void ledblink(void);
int main(void)
{
	disable_global_int();
	setup_leds();
	setup_systick();
	init_scheduler();
	sys_ringbuf_uchar_init();
	setup_uart0();
	setup_keyboard();
	setup_buttons();
	setup_clock();
	setup_delay(); //Used by lcd.
	setup_lcd0();
	setup_encoder0();
	enable_global_int();
	add_task(check_keyboard);
	add_task(collect_button_events);
	add_task(run_clock);
	add_task(display_clock);
	add_task(clock_uart0_controller);
	add_task(clock_keyboard_controller);
	add_task(scan_encoder0);
	add_task(alarm_task);
	start_scheduler();

	return 0;
}
