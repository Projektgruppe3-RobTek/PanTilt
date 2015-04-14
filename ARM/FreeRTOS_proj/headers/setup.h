/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: setup.h
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: Contains the different functions used in setup.c
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150212  MS    			Syntax Fixed.
* 150217  StefanRvO 	Split setup into multiple functions.
*****************************************************************************/
#pragma once

/***************************** Include files ********************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "GLOBAL_DEFINITIONS.h"

/*****************************   Functions   ********************************/
static void enable_global_int(void)  __attribute__ ((unused));
static void disable_global_int(void) __attribute__ ((unused));

static void enable_global_int()
/**********************************************
* Input : None.
* Output : None.
* Function : Enable global interrupt.
**********************************************/
{
  __asm("cpsie i");	// Enable interrupts.
}

static void disable_global_int()
/**********************************************
* Input : None.
* Output : None.
* Function : Disable global interrupt.
**********************************************/
{
  __asm("cpsid i");	// Disable interrupts.
}

/****************************** End of module *******************************/
