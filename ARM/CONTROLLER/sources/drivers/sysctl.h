#pragma once

#include "../../headers/tm4c123gh6pm.h"
#include "../../headers/emp_type.h"

#define XTAL_CLK SYSCTL_RCC_XTAL_16MHZ
#define PLL_CLOCK 400000 // PLL clock in KHz
void set_sysclk(void); //Freq is requested frequency in Khz. May not be set excatly to this.

void do_reset(void);
