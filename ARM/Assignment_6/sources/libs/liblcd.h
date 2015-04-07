/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: lcd.h
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: Functions for initialize the LCD display and using it.
*
* 						 This is a port of the arduino library LiquidCrystal.
* 						 The library has been ported to C for use with the ARM Cortex M4.
*              It should be very easy to port to other architectures.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150224  StefanRvo		Created file.
* 150226	MS 					Fixed syntax.
*****************************************************************************/
#pragma once

/***************************** Include files ********************************/
#include <stdio.h>
#include <string.h>
#include "../os/SWDelay.h"

/*******************************   Defines    *******************************/
// Commands.
#define LCD_CLEARDISPLAY    0x01
#define LCD_RETURNHOME 		  0x02
#define LCD_ENTRYMODESET 	  0x04
#define LCD_DISPLAYCONTROL  0x08
#define LCD_CURSORSHIFT     0x10
#define LCD_FUNCTIONSET     0x20
#define LCD_SETCGRAMADDR    0x40
#define LCD_SETDDRAMADDR    0x80

// Flags for display entry mode.
#define LCD_ENTRYRIGHT 						0x00
#define LCD_ENTRYLEFT 						0x02
#define LCD_ENTRYSHIFTINCREMENT 	0x01
#define LCD_ENTRYSHIFTDECREMENT 	0x00

// Flags for display on/off control.
#define LCD_DISPLAYON 	0x04
#define LCD_DISPLAYOFF 	0x00
#define LCD_CURSORON 		0x02
#define LCD_CURSOROFF 	0x00
#define LCD_BLINKON 		0x01
#define LCD_BLINKOFF 		0x00

// Flags for display/cursor shift.
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 	0x00
#define LCD_MOVERIGHT 	0x04
#define LCD_MOVELEFT 		0x00

// Flags for function set.
#define LCD_8BITMODE 		0x10
#define LCD_4BITMODE 		0x00
#define LCD_2LINE 			0x08
#define LCD_1LINE				0x00
#define LCD_5x10DOTS 		0x04
#define LCD_5x8DOTS 		0x00

//Line adress offsets. These can be found at http://www.vishay.com/docs/37314/lcd020n004l.pdf
//Page 1.
#define LINE0_OFFSET 0x00
#define LINE1_OFFSET 0x40
#define LINE2_OFFSET 0x14
#define LINE3_OFFSET 0x54

/*******************************   Structs    *******************************/
typedef struct {
  INT32U _rs_pin; // LOW: command.  HIGH: character.
  INT32U _enable_pin; // Activated by a HIGH pulse.
  volatile INT32U *_rs_enable_port;
  INT32U _data_pins[8];
  volatile INT32U *_data_port;
  INT8U _displayfunction;
  INT8U _displaycontrol;
  INT8U _displaymode;
  INT8U _initialized;
  INT8U _numlines;
  INT8U _currline;
  } lcd;

/*************************** Initialize Functions ***************************/
lcd lcd_init(INT8U fourbitmode, 		 INT32U rs, INT32U enable, volatile INT32U *	rs_enable_port,
						 INT32U d0, 						 INT32U d1, INT32U d2, 		 INT32U d3,
						 INT32U d4, INT32U d5, INT32U d6, INT32U d7, 		 volatile INT32U *data_port);

lcd lcd_init_4bit(INT32U rs, INT32U enable, volatile INT32U *rs_enable_port,
									INT32U d0, INT32U d1, 		INT32U d2,
									INT32U d3, INT32U volatile *data_port);

lcd lcd_init_8bit(INT32U rs, INT32U enable, volatile INT32U *rs_enable_port,
									INT32U d0, INT32U d1, 		INT32U d2, INT32U d3,
									INT32U d4, INT32U d5, 		INT32U d6, INT32U d7,
									volatile INT32U *data_port);

/*****************************   Functions   ********************************/
void lcd_begin(lcd *lcd_s, INT8U lines);
void lcd_home(lcd *lcd_s); 	// This takes a long time. Needs 1,6 ms before next command.
void lcd_clear(lcd *lcd_s); // This takes a long time. Needs 1,6 ms before next command.

void lcd_display(lcd *lcd_s);
void lcd_no_display(lcd *lcd_s);

void lcd_blink(lcd *lcd_s);
void lcd_no_blink(lcd *lcd_s);

void lcd_cursor(lcd *lcd_s);
void lcd_no_cursor(lcd *lcd_s);
void lcd_set_cursor(lcd *lcd_s, INT8U col, INT8U row);

void lcd_autoscroll(lcd *lcd_s);
void lcd_no_autoscroll(lcd *lcd_s);
void lcd_scroll_display_left(lcd *lcd_s);
void lcd_scroll_display_right(lcd *lcd_s);

void lcd_left_to_right(lcd *lcd_s);
void lcd_right_to_left(lcd *lcd_s);

void lcd_write(lcd *lcd_s, INT8U value);
void lcd_write_4_bits(lcd *lcd_s, INT8U value);
void lcd_write_8_bits(lcd *lcd_s, INT8U value);
void lcd_write_string(lcd *lcd_s, char *string);

void lcd_create_char(lcd *lcd_s, INT8U location, INT8U charmap[]);

void lcd_send(lcd *lcd_s, INT8U value, INT8U mode);
void lcd_command(lcd *lcd_s, INT8U value);

void lcd_pulse_enable(lcd *lcd_s);

/****************************** End of module *******************************/
