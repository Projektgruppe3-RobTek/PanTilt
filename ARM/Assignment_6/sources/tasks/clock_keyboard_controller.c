#include "clock_keyboard_controller.h"
#include "clock.h"
#include "../os/scheduler.h"
#include "../drivers/keyboard.h"
#include "../drivers/lcd0.h"
#include <stdbool.h>

typedef enum {
	SET_MIN_UNLOCKED,
	SET_MIN_LOCKED,
	SET_HOUR_UNLOCKED,
	SET_HOUR_LOCKED,
	IDLE,
} controller_mode;

static void release_lcd(void);

static void set_min(controller_mode *state, time *clock, INT8U *cursor_position)
{
	lcd0_set_cursor(*cursor_position+3, 0);
	if(keyboard_data_available())
	{
		INT8U key = keyboard_in_char();
		if(key == '#')
		{
			set_clock(clock->hour, clock->min, 0);
			*cursor_position = 0;
			*state = IDLE;
			release_lcd();
		}
		else if(key == '*')
		{
			set_clock(clock->hour, clock->min, 0);
			*cursor_position = 0;
			*state = SET_HOUR_UNLOCKED;
			release_lcd();
		}
		if(*cursor_position == 0 && key >= '0' && key <= '5')
		{
			lcd0_write_char(key);
			clock->min = clock->min % 10 + (key - '0') * 10;
			(*cursor_position)++;
		}
		else if(*cursor_position == 1 && key >= '0' && key <= '9')
		{
			lcd0_write_char(key);
			clock->min = (clock->min / 10) * 10 + (key - '0');
			(*cursor_position)--;
			set_clock(clock->hour, clock->min, 0);
			release_lcd();
			*state = IDLE;
		}
	}
}
static void set_hour(controller_mode *state, time *clock, INT8U *cursor_position)
{
	lcd0_set_cursor(*cursor_position, 0);
	if(keyboard_data_available())
	{
		INT8U key = keyboard_in_char();
		if(key == '*')
		{
			set_clock(clock->hour, clock->min, 0);
			*cursor_position = 0;
			*state = IDLE;
			release_lcd();
		}
		else if(key == '#')
		{
			set_clock(clock->hour, clock->min, 0);
			*cursor_position = 0;
			*state = SET_MIN_UNLOCKED;
			release_lcd();
		}
		else if(*cursor_position == 0 && key >= '0' && key <= '2')
		{
			lcd0_write_char(key);
			clock->hour = clock->hour % 10 + (key - '0') * 10;
			(*cursor_position)++;
		}
		else if(*cursor_position == 1 && (
						((clock->hour / 10) == 2 && key >= '0' && key <= '3') ||
						((clock->hour / 10) < 2 && key >= '0' && key <= '9')))
		{
			lcd0_write_char(key);
			clock->hour = (clock->hour / 10) * 10 + (key - '0');
			(*cursor_position)--;
			set_clock(clock->hour, clock->min, 0);
			release_lcd();
			*state = IDLE;
		}
	}
}



void clock_keyboard_controller(void)
{
	static INT8U cursor_position = 0;
	static time clock;
	static controller_mode state = IDLE;
	switch(state)
	{
		case IDLE:
			if(keyboard_data_available())
			{
				INT8U key = keyboard_in_char();
				if (key == '*')
				{
					state = SET_HOUR_UNLOCKED;
					clock = get_clock();
				}
				else if(key == '#')
				{
					state = SET_MIN_UNLOCKED;
					clock = get_clock();
				}
			}
		break;
		case SET_MIN_UNLOCKED:
			if(check_release(lcd0_available))
			{
				lcd0_lock();
				state = SET_MIN_LOCKED;
				lcd0_set_cursor(7, 0);
				lcd0_write_string("SET MIN");
				lcd0_set_cursor(0, 1);
				lcd0_write_string("8====3     (o.O)"); //Very important!
				lcd0_cursor();
				lcd0_blink();
				//There should not be a break here!
			}
			else
				break;
		case SET_MIN_LOCKED:
			set_min(&state, &clock, &cursor_position);
		break;

		case SET_HOUR_UNLOCKED:
			if(check_release(lcd0_available))
			{
				lcd0_lock();
				state = SET_HOUR_LOCKED;
				lcd0_set_cursor(7, 0);
				lcd0_write_string("SET HOUR");
				lcd0_set_cursor(0, 1);
				lcd0_write_string("8====D   (.)Y(.)"); //Very important!
				lcd0_cursor();
				lcd0_blink();
					//There should not be a break here!
			}
			else
				break;
		case SET_HOUR_LOCKED:
			set_hour(&state, &clock, &cursor_position);
		break;

		default:
		break;
	}
}

static void release_lcd(void)
{
	lcd0_no_blink();
	lcd0_no_cursor();
	lcd0_set_cursor(7, 0);
	lcd0_write_string("        ");
	lcd0_set_cursor(0, 1);
	lcd0_write_string("        ");
	lcd0_unlock();
}
