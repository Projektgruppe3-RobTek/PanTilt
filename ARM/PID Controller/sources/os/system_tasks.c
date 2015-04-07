#include "system_tasks.h"
#include "../../headers/tm4c123gh6pm.h"
#include "systick.h"
#include "../../headers/emp_type.h"
#include "system_timers.h"
#include "../drivers/leds.h"
static void blink_status_led(void);
static void count_system_timers(void);

static void blink_status_led()
{
	static INT16U status_led_timer = STATUS_BLINK_TIME / TIMEOUT_SYSTICK;
	if(status_led_timer > 0)
		status_led_timer--;
	if(!status_led_timer)
		{
			LED_STATUS_PORT ^= LED_STATUS;
			status_led_timer = STATUS_BLINK_TIME / TIMEOUT_SYSTICK;
		}
}

static void count_system_timers()
{
	for (INT8U i = 0; i < MAX_TIMERS; i += 1)
	{
		count_timer(i);
	}
}

void system_tasks()
{
	blink_status_led();
	count_system_timers();
}
