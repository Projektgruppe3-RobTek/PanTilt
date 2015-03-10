#include "../headers/systick.h"

volatile INT32U ticks = 0;



void systick_timer_isr(void)
/**********************************************
* Input : None.
* Output : None.
* Function : Increment systick variable.
**********************************************/
{
	// Hardware clears systick int reguest.
	ticks++;
}


void setup_systick(void)
/**********************************************
* Input : None.
* Output : None.
* Function : Setup systick timer.
**********************************************/
{
	// Setup systick timer.
  NVIC_ST_CTRL_R 	 &= ~(NVIC_ST_CTRL_ENABLE);	// Disable systick timer.

  // Set current systick counter to reload value.
  NVIC_ST_CURRENT_R = SYSTICK_RELOAD_VALUE;

  // Set Reload value, Systick reload register.
  NVIC_ST_RELOAD_R  = SYSTICK_RELOAD_VALUE;

  // NVIC systick setup, vector number 15.
  // Clear pending systick interrupt request.
  NVIC_INT_CTRL_R  |= NVIC_INT_CTRL_PENDSTCLR;

  // Select systick clock source, Use core clock.
  NVIC_ST_CTRL_R   |= NVIC_ST_CTRL_CLK_SRC;
	
  // Enable systick interrupt.
  NVIC_ST_CTRL_R   |= NVIC_ST_CTRL_INTEN;
  NVIC_ST_CTRL_R 	 |= NVIC_ST_CTRL_ENABLE;		// Enable and start timer.
}
