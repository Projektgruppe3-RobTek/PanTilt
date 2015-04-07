#pragma once

#include "../libs/liblcd.h"
#include <stdbool.h>
bool lcd0_available(void);
void lcd0_lock(void);
void lcd0_unlock(void);
bool lcd0_try_lock(void);

void setup_lcd0(void);
void lcd0_home(void);
void lcd0_clear(void);
void lcd0_display(void);
void lcd0_display(void);
void lcd0_no_display(void);

void lcd0_blink(void);
void lcd0_no_blink(void);

void lcd0_cursor(void);
void lcd0_no_cursor(void);
void lcd0_set_cursor(INT8U col, INT8U row);

void lcd0_autoscroll(void);
void lcd0_no_autoscroll(void);
void lcd0_scroll_display_left(void);
void lcd0_scroll_display_right(void);

void lcd0_left_to_right(void);
void lcd0_right_to_left(void);
void lcd0_write_char(INT8U value);
void lcd0_write_string(char *string);
void lcd0_create_char(INT8U location, INT8U charmap[]);

#define LCD_D4			1<<4
#define LCD_D5			1<<5
#define LCD_D6			1<<6
#define LCD_D7			1<<7
#define LCD_RS			1<<2
#define LCD_E				1<<3

#define LCD_DATA_PORT 		GPIO_PORTC_DATA_R
#define LCD_RS_E_PORT			GPIO_PORTD_DATA_R
