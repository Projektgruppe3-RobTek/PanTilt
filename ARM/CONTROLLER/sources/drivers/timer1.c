#include "timer1.h"

#define TIMER_FREQUENCY 1000
#define INTERRUPT_INTERVAL (1000000 / TIMER_FREQUENCY)//interval in µs
#define TAILR_VAL (FCPU / 1000000 * INTERRUPT_INTERVAL - 1)
#include "leds.h"
#include "../../headers/tm4c123gh6pm.h"
#include "../../headers/GLOBAL_DEFINITIONS.h"
#include "UART.h"

void setup_timer1()
{
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER1; 								        // Enable the timer hardware.
	__asm__("NOP");																				        // Waste a few cycles.
	__asm__("NOP");
	TIMER1_CTL_R 	 = 0;																	         // Disable the timer.
	TIMER1_CFG_R 		= 0; 																	       // 32 bit mode (no RTC).
	TIMER1_TAMR_R		= TIMER_TAMR_TAMR_PERIOD;										 // down, periodic.

  TIMER1_TAILR_R  = TAILR_VAL;                                  //Set trigger value
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;														//clear interrupt flags

  TIMER1_IMR_R   =  TIMER_IMR_TATOIM;         									//Enable timer a interrupts
  NVIC_EN0_R |= (1 << (INT_TIMER1A-16));                     		//enable interrupt
  TIMER1_CTL_R |= TIMER_CTL_TAEN; // Enable TIMER1_A/sampling.

}
