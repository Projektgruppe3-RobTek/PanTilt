
#pragma once

#include "../../headers/tm4c123gh6pm.h"
#include "../../headers/emp_type.h"
#include "../os/system_buffers.h"
#include "../../headers/GLOBAL_DEFINITIONS.h"
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
RBUF_INDEX_TYPE uart0_data_available(void);
RBUF_INDEX_TYPE uart0_in_space_left(void);
RBUF_INDEX_TYPE uart0_out_space_left(void);
void setup_uart0(void);
void uart0_isr(void);
void uart0_in_clear(void);
void uart0_out_clear(void);
