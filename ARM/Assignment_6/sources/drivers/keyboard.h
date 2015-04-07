#pragma once

#include "../os/system_buffers.h"
#include "../../headers/emp_type.h"
#include "../../headers/tm4c123gh6pm.h"
void setup_keyboard(void);
void check_keyboard(void);
RBUF_INDEX_TYPE keyboard_data_available(void);
INT8U keyboard_in_char(void);

#define KEYBOARD_X1 (1<<4)
#define KEYBOARD_X2 (1<<3)
#define KEYBOARD_X3 (1<<2)
#define KEYBOARD_Y1 (1<<3)
#define KEYBOARD_Y2 (1<<2)
#define KEYBOARD_Y3 (1<<1)
#define KEYBOARD_Y4 (1<<0)
#define KEYBOARD_IN_M (0xF)

#define KEYBOARD_REPEAT_TIME 500 //in ms
#define KEYBOARD_CHECK_PERIOD 5   //in ms
