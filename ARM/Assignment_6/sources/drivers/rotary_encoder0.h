#pragma once
#include "../../headers/tm4c123gh6pm.h"
#include "../../headers/emp_type.h"
#include <stdbool.h>


#define DIGI_A  (1 << 5)
#define DIGI_B  (1 << 6)
#define DIGI_P2 (1 << 7)
INT32S get_encoder0_pos(void);
void scan_encoder0(void);
void setup_encoder0(void);
void reset_encoder0(void);
bool encoder0_pushed(void);
void encoder0_consume_push(void);
