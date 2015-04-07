/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: lcd.c
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: See header.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150217  StefanRvo		Created file.
* 150226	MS 					Fixed syntax.
*****************************************************************************/

/*****************************  Description  ********************************/
// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data.
//    N  = 0; 1-line display.
//    F  = 0; 5x8 dot character font.
// 3. Display on/off control:
//    D = 0; Display off.
//    C = 0; Cursor off.
//    B = 0; Blinking off.
// 4. Entry mode set:
//    I/D = 1; Increment by 1.
//    S = 0; No shift.
//
// Note, however, that resetting the device doesn't reset the LCD, so we
// can't assume that its in that state when powered up.

/***************************** Include files ********************************/
#include "liblcd.h"

/*************************** Initialize Functions ***************************/
lcd lcd_init(INT8U fourbitmode, INT32U rs, INT32U enable, volatile INT32U *	rs_enable_port,
						 INT32U d0, 				INT32U d1, INT32U d2, 		INT32U d3,
						 INT32U d4, 				INT32U d5, INT32U d6, 		INT32U d7,
						 volatile INT32U *data_port)
/*****************************************************
* Input : mode. are we running 4 bit?,rs pin,
					enable pin ,port for rs_pin and enable_pin,
* 				data0 pin, data1 pin ,data2 pin,
* 				data3 pin, data4 pin, data5 pin,
*					data6 pin, data7 pin, port for data pins.
* Output : lcd struct.
* Function : Initialises a lcd struct to 4 or 8 bit mode and returns it.
******************************************************/
{
	lcd lcd_s;
	lcd_s._rs_pin = rs;
	lcd_s._enable_pin = enable;
	lcd_s._rs_enable_port = rs_enable_port;
	lcd_s._data_pins[0] = d0;
	lcd_s._data_pins[1] = d1;
	lcd_s._data_pins[2] = d2;
	lcd_s._data_pins[3] = d3;
	lcd_s._data_pins[4] = d4;
	lcd_s._data_pins[5] = d5;
	lcd_s._data_pins[6] = d6;
	lcd_s._data_pins[7] = d7;
	lcd_s._data_port = data_port;

	// Don't setup pins, should be done beforehand.

	if(fourbitmode)
		lcd_s._displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	else
		lcd_s._displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;

	return lcd_s;
}

lcd lcd_init_4bit(INT32U rs, INT32U enable, volatile INT32U *rs_enable_port,
									INT32U d0, INT32U d1, 		INT32U d2,
									INT32U d3, INT32U volatile *data_port)
/**********************************************
* Input : rs pin, enable pin ,port for rs_pin and enable_pin,
* 				data0 pin, data1 pin ,data2 pin ,
* 				data3 pin, port for data pins.
* Output : lcd struct.
* Function : Initialises a lcd struct to 4 bit mode and returns it.
**********************************************/
{
	return lcd_init(1, rs, enable, rs_enable_port, d0, d1, d2, d3, 0, 0, 0, 0, data_port);
}

lcd lcd_init_8bit(INT32U rs, INT32U enable, volatile INT32U *rs_enable_port,
								INT32U d0, INT32U d1, 		INT32U d2,
								INT32U d3, INT32U d4, 		INT32U d5,
								INT32U d6, INT32U d7,	 	  volatile INT32U *data_port)
/**********************************************
* Input : rs pin, enable pin ,port for rs_pin and enable_pin,
* 				data0 pin, data1 pin ,data2 pin ,
* 				data3 pin, data4 pin, data5 pin,
*					data6 pin, data7 pin, port for data pins.
* Output : lcd struct.
* Function : initialises a lcd struct to 8 bit mode and returns it.
**********************************************/
{
	return lcd_init(0, rs, enable, rs_enable_port, d0, d1, d2, d3, d4, d5, d6, d7, data_port);
}

/*****************************   Functions   ********************************/
void lcd_begin(lcd *lcd_s, INT8U lines)
/**********************************************
* Input : Pointer to lcd, does the lcd hav 2 lines?
* Output : None
* Function : sets up the lcd display, clears it,
	and sets cursor to (0,0).
**********************************************/
{
	INT8U dotsize = LCD_5x8DOTS;

	if(lines > 1)
		lcd_s->_displayfunction |= LCD_2LINE;

	lcd_s->_numlines = lines;
	lcd_s->_currline = 0;

	// For some 1 line displays you can select a 10 pixel high font.
	if((dotsize != 0) && (lines == 1))
		lcd_s->_displayfunction |= LCD_5x10DOTS;

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// According to datasheet, we need at least 40ms
	// after power rises above 2.7V before sending commands.
	delay_microseconds(40000);

	// Now we pull both RS and R/W low to begin commands.
	*(lcd_s->_rs_enable_port) &= ~(lcd_s->_rs_pin);
	*(lcd_s->_rs_enable_port) &= ~(lcd_s->_enable_pin);

	// Put the LCD into 4 bit or 8 bit mode
	// this is according to the DEM 16216 SYH-PY datasheet
	// figure 11-2, pg 13.

	if (!(lcd_s->_displayfunction & LCD_8BITMODE)) // 4 bit setup.
	{
		// This is according to the hitachi HD44780 datasheet
		// figure 24, pg 46.

		// We start in 8bit mode, try to set 4 bit mode.
		lcd_write_4_bits(lcd_s, 0x03);
		delay_microseconds(4500); // Wait min 4.1ms.

		// Second try
		lcd_write_4_bits(lcd_s, 0x03);
		delay_microseconds(4500); // Wait min 4.1ms.

		// Third go!
		lcd_write_4_bits(lcd_s, 0x03);
		delay_microseconds(150);

		// Finally, set to 4-bit interface.
		lcd_write_4_bits(lcd_s, 0x02);
		// Need to wait 37 µs between commands.
		delay_microseconds(37);
	}
	else  // 8 bit setup
	{
		// This is according to the hitachi HD44780 datasheet
		// page 45 figure 23.

		// Send function set command sequence
		lcd_command(lcd_s, LCD_FUNCTIONSET | lcd_s->_displayfunction);
		delay_microseconds(4500);  // Wait more than 4.1ms.

		// Second try.
		lcd_command(lcd_s, LCD_FUNCTIONSET | lcd_s->_displayfunction);
		delay_microseconds(150);

		// Third go.
		lcd_command(lcd_s, LCD_FUNCTIONSET | lcd_s->_displayfunction);
	}

	// Finally, set # lines, font size, etc.
	lcd_command(lcd_s, LCD_FUNCTIONSET | lcd_s->_displayfunction);

	// Turn the display on with no cursor or blinking default.
	lcd_s->_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	lcd_display(lcd_s);

	// Clear it off.
	lcd_clear(lcd_s);
	delay_microseconds(2000);

	// Initialize to default text direction (for romance languages).
	lcd_s->_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// Set the entry mode.
	lcd_command(lcd_s, LCD_ENTRYMODESET | lcd_s->_displaymode);
}

/******************* High level commands, for the user **********************/
void lcd_home(lcd *lcd_s)
/**********************************************
* Input : Pointer to lcd.
* Output : None.
* Function : Set cursor position to zero.
**********************************************/
{
	lcd_command(lcd_s, LCD_RETURNHOME);
}

void lcd_clear(lcd *lcd_s)
/**********************************************
* Input : Pointer to lcd.
* Output : None.
* Function : Clear display, set cursor position to zero.
**********************************************/
{
	lcd_command(lcd_s, LCD_CLEARDISPLAY);
}

void lcd_display(lcd *lcd_s)
/**********************************************
* Input : Pointer to lcd.
* Output : None.
* Function : Turn the display on (quickly).
**********************************************/
{
	lcd_s->_displaycontrol |= LCD_DISPLAYON;
	lcd_command(lcd_s, LCD_DISPLAYCONTROL | lcd_s->_displaycontrol);
}

void lcd_no_display(lcd *lcd_s)
/**********************************************
* Input : Pointer to lcd.
* Output : None.
* Function : Turn the display off (quickly).
**********************************************/
{
	lcd_s->_displaycontrol &= ~LCD_DISPLAYON;
	lcd_command(lcd_s, LCD_DISPLAYCONTROL | lcd_s->_displaycontrol);
}

void lcd_blink(lcd *lcd_s)
/**********************************************
* Input : Pointer to lcd.
* Output : None.
* Function : Turn on the blinking cursor.
**********************************************/
{
	lcd_s->_displaycontrol |= LCD_BLINKON;
	lcd_command(lcd_s, LCD_DISPLAYCONTROL | lcd_s->_displaycontrol);
}

// Turn on and off the blinking cursor
void lcd_no_blink(lcd *lcd_s)
/**********************************************
* Input : Pointer to lcd.
* Output : None.
* Function : Turn off the blinking cursor.
**********************************************/
{
	lcd_s->_displaycontrol &= ~LCD_BLINKON;
	lcd_command(lcd_s, LCD_DISPLAYCONTROL | lcd_s->_displaycontrol);
}

void lcd_cursor(lcd *lcd_s)
/**********************************************
* Input : Pointer to lcd.
* Output : None.
* Function : Turns the underline cursor on.
**********************************************/
{
	lcd_s->_displaycontrol |= LCD_CURSORON;
	lcd_command(lcd_s, LCD_DISPLAYCONTROL | lcd_s->_displaycontrol);
}

void lcd_no_cursor(lcd *lcd_s)
/**********************************************
* Input : Pointer to lcd.
* Output : None.
* Function : Turns the underline cursor off.
**********************************************/
{
	lcd_s->_displaycontrol &= ~LCD_CURSORON;
	lcd_command(lcd_s, LCD_DISPLAYCONTROL | lcd_s->_displaycontrol);
}

void lcd_set_cursor(lcd *lcd_s, INT8U col, INT8U row)
/**********************************************
* Input : Pointer to lcd, column position, row position.
* Output : None.
* Function : Set cursor to input location.
**********************************************/
{
	row %= lcd_s->_numlines;
	INT8U row_offsets[] = {LINE0_OFFSET, LINE1_OFFSET, LINE2_OFFSET, LINE3_OFFSET};
	if(row > lcd_s->_numlines) {
		 row = lcd_s->_numlines-1;    // We count rows starting w/0.
	}

	lcd_s->_currline = row;

	lcd_command(lcd_s, LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void lcd_autoscroll(lcd *lcd_s)
/**********************************************
* Input : Pointer to lcd.
* Output : None.
* Function : This will 'right justify' text from the cursor
**********************************************/
{
	lcd_s->_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	lcd_command(lcd_s, LCD_ENTRYMODESET | lcd_s->_displaymode);
}

void lcd_no_autoscroll(lcd *lcd_s)
/**********************************************
* Input : Pointer to lcd.
* Output : None.
* Function : This will 'left justify' text from the cursor.
**********************************************/
{
	lcd_s->_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	lcd_command(lcd_s, LCD_ENTRYMODESET | lcd_s->_displaymode);
}

void lcd_scroll_display_left(lcd *lcd_s)
/**********************************************
* Input : Pointer to lcd.
* Output : None.
* Function : Scroll the display left without changing the RAM.
**********************************************/
{
	lcd_command(lcd_s, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void lcd_scroll_display_right(lcd *lcd_s)
/**********************************************
* Input : Pointer to lcd.
* Output : None.
* Function : Scroll the display right without changing the RAM.
**********************************************/
{
	lcd_command(lcd_s, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void lcd_left_to_right(lcd *lcd_s)
/**********************************************
* Input : Pointer to lcd.
* Output : None.
* Function : This is for text that flows Left to Right.
**********************************************/
{
	lcd_s->_displaymode |= LCD_ENTRYLEFT;
	lcd_command(lcd_s, LCD_ENTRYMODESET | lcd_s->_displaymode);
}

void lcd_right_to_left(lcd *lcd_s)
/**********************************************
* Input : Pointer to lcd.
* Output : None.
* Function : This is for text that flows Right to Left.
**********************************************/
{
	lcd_s->_displaymode &= ~LCD_ENTRYLEFT;
	lcd_command(lcd_s, LCD_ENTRYMODESET | lcd_s->_displaymode);
}

void lcd_create_char(lcd *lcd_s, INT8U location, INT8U charmap[])
/**********************************************
* Input : Pointer to lcd, location col/row, desired char.
* Output : None.
* Function : Allows us to fill the first 8 CGRAM locations
* 					 with custom characters.
**********************************************/
{
	location &= 0x7; // We only have 8 locations 0-7.
	lcd_command(lcd_s, LCD_SETCGRAMADDR | (location << 3));

	for(INT8U i = 0; i < 8; i++)
	{
		lcd_write(lcd_s, charmap[i]);
	}
}

void lcd_write_string(lcd *lcd_s, char *string)
/**********************************************
* Input : Pointer to lcd, string.
* Output : None.
* Function : Write string from current position and forward.
**********************************************/
{
	INT8U i = 0;
	while(string[i])
	{
		lcd_write(lcd_s, (unsigned char)string[i]);
		i++;
	}
}

/*************** Mid level commands, for sending data/cmds ******************/
void lcd_command(lcd *lcd_s, INT8U value)
/**********************************************
* Input : Pointer to lcd, command to send .
* Output : None.
* Function : sends a command to the LCD.
**********************************************/
{
	lcd_send(lcd_s, value, 0);
}

void lcd_write(lcd *lcd_s, INT8U value)
/**********************************************
* Input : Pointer to lcd, char to write .
* Output : None.
* Function : writes a char to the display.
**********************************************/
{
	if(value == '\n') lcd_set_cursor(lcd_s, 0, lcd_s->_currline+1);
	else lcd_send(lcd_s, value, 1);
}

/****************** Low level commands, for pushing data ********************/
void lcd_pulse_enable(lcd *lcd_s)
/**********************************************
* Input : Pointer to lcd.
* Output : None.
* Function : Toggle enable pin.
**********************************************/
{
	*(lcd_s->_rs_enable_port) |=  lcd_s->_enable_pin;
	*(lcd_s->_rs_enable_port) &= ~lcd_s->_enable_pin;
}

void lcd_write_4_bits(lcd *lcd_s, INT8U value)
/**********************************************
* Input : Pointer to lcd, .
* Output : None.
* Function : writes four bits to the display .
**********************************************/
{
	for(INT8U i = 0; i < 4; i++)
	{
		if((value >> i) & 0x01) // Set pin high.
			*(lcd_s->_data_port) |= lcd_s->_data_pins[i];
		else 											// Set pin low.
			*(lcd_s->_data_port) &= ~lcd_s->_data_pins[i];
	}
	lcd_pulse_enable(lcd_s);
}

void lcd_write_8_bits(lcd *lcd_s, INT8U value)
/**********************************************
* Input : Pointer to lcd,
* Output : None.
* Function : writes 8 bit to the display.
**********************************************/
{
	for(INT8U i = 0; i < 8; i++)
	{
		if((value >> i) & 0x01) // Set pin high.
			*(lcd_s->_data_port) |= lcd_s->_data_pins[i];
		else 											// Set pin low.
			*(lcd_s->_data_port) &= ~lcd_s->_data_pins[i];
	}

	lcd_pulse_enable(lcd_s);
}

void lcd_send(lcd *lcd_s, INT8U value, INT8U mode)
/**********************************************
* Input : Pointer to lcd, , .
* Output : None.
* Function : Write either command or data,
* 					 with automatic 4/8-bit selection.
**********************************************/
{
	if(mode)
		*(lcd_s->_rs_enable_port) |= lcd_s->_rs_pin;
	else
		*(lcd_s->_rs_enable_port) &= ~lcd_s->_rs_pin;

	if(lcd_s->_displayfunction & LCD_8BITMODE)
	{
		lcd_write_8_bits(lcd_s, value);
	}
	else
	{
		lcd_write_4_bits(lcd_s, value>>4); // Send 4 high bits.
		lcd_write_4_bits(lcd_s, value);		 // Send 4 low bits.
	}
	delay_microseconds(37); 				 		 // Need to wait 37 µs between commands.
}

/****************************** End of module *******************************/
