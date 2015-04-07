#include "lcd0.h"
#include "../../headers/GLOBAL_DEFINITIONS.h"
#include "../os/mutex.h"
lcd lcd0;
mutex_t lcd0_mutex = UNLOCKED;

bool lcd0_available(void)
{
	return is_mutex_unlocked(&lcd0_mutex);
}
bool lcd0_try_lock(void)
{
	return try_lock_mutex(&lcd0_mutex);
}
void lcd0_lock(void)
{
	lock_mutex(&lcd0_mutex);
}

void lcd0_unlock(void)
{
	unlock_mutex(&lcd0_mutex);
}



void setup_lcd0(void)
{
	SYSCTL_RCGC2_R 		|= SYSCTL_RCGC2_GPIOC;
  SYSCTL_RCGC2_R 		|= SYSCTL_RCGC2_GPIOD;

  __asm__("NOP");
	__asm__("NOP");

	 GPIO_PORTC_DIR_R |= LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
	 GPIO_PORTD_DIR_R |= LCD_E  | LCD_RS;

	 GPIO_PORTC_DEN_R |= LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
	 GPIO_PORTD_DEN_R |= LCD_E  | LCD_RS;

	lcd0 = lcd_init_4bit(LCD_RS, LCD_E, (volatile INT32U *)&LCD_RS_E_PORT, LCD_D4,
															 LCD_D5, LCD_D6, LCD_D7, (volatile INT32U *)&LCD_DATA_PORT);
	lcd_begin(&lcd0, 2);
}

void lcd0_home(void)
{
	lcd_home(&lcd0);
}

void lcd0_clear(void)
{
	lcd_clear(&lcd0);
}

void lcd0_display(void)
{
	lcd_display(&lcd0);
}

void lcd0_no_display(void)
{
	lcd_no_display(&lcd0);
}

void lcd0_blink(void)
{
	lcd_blink(&lcd0);
}
void lcd0_no_blink(void)
{
	lcd_no_blink(&lcd0);
}
void lcd0_cursor(void)
{
	lcd_cursor(&lcd0);
}
void lcd0_no_cursor(void)
{
	lcd_no_cursor(&lcd0);
}

void lcd0_set_cursor(INT8U col, INT8U row)
{
	lcd_set_cursor(&lcd0, col, row);
}

void lcd0_autoscroll(void)
{
	lcd_autoscroll(&lcd0);
}

void lcd0_no_autoscroll(void)
{
	lcd_no_autoscroll(&lcd0);
}

void lcd0_scroll_display_left(void)
{
	lcd_scroll_display_left(&lcd0);
}

void lcd0_scroll_display_right(void)
{
	lcd_scroll_display_right(&lcd0);
}

void lcd0_left_to_right(void)
{
	lcd_left_to_right(&lcd0);
}

void lcd0_right_to_left(void)
{
	lcd_right_to_left(&lcd0);
}
void lcd0_write_char(INT8U value)
{
	lcd_write(&lcd0, value);
}

void lcd0_write_string(char *string)
{
	lcd_write_string(&lcd0, string);
}

void lcd0_create_char(INT8U location, INT8U charmap[])
{
	lcd_create_char(&lcd0, location, charmap);
}
