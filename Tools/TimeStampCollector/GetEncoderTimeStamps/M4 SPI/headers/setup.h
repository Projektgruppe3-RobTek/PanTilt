#ifndef _SETUP_H
#define _SETUP_H

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "GLOBAL_DEFINITIONS.h"

#define SYSTICK_RELOAD_VALUE	FCPU / 1000 * TIMEOUT_SYSTICK - 1

#if (SYSTICK_RELOAD_VALUE > 0xFFFFFF)
	#error "SYSTICK_RELOAD_VALUE is too high"
#endif

void setup_gpio(void);
__attribute__((unused)) static void enable_global_int(void);
__attribute__((unused)) static void disable_global_int(void);

static void enable_global_int()
{
  __asm("cpsie i");	// Enable interrupts.
}

static void disable_global_int()
{
  __asm("cpsid i");	// Disable interrupts.
}
#endif
