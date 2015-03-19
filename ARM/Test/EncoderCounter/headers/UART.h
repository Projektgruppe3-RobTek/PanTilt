// UART.h
// Runs on LM3S811, LM3S1968, LM3S8962, LM4F120
// Simple device driver for the UART.
// Daniel Valvano
// June 17, 2013

/* This example accompanies the books
  "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
  ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013

"Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013
 
 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// U0Rx (VCP receive) connected to PA0
// U0Tx (VCP transmit) connected to PA1

#ifndef _UART_H_
#define _UART_H_

#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "system_buffers.h"
#include "GLOBAL_DEFINITIONS.h"
#include <string.h>
// standard ASCII symbols
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F

#define BRD 115200
#define BRD_BASE (FCPU * 64 / (16 * BRD)) //BRD integer part
#define IBRD (BRD_BASE / 64)
#define FBRD BRD_BASE - IBRD * 64



void uart0_out_string(char *string);
char uart0_in_char(void);
void uart0_out_char(char data);
RBUF_INDEX_TYPE uart0_data_avaliable(void);
RBUF_INDEX_TYPE uart0_in_space_left(void);
RBUF_INDEX_TYPE uart0_out_space_left(void);
void setup_uart0(void);
void uart0_isr(void);
void uart0_in_clear(void);
void uart0_out_clear(void);

#endif
