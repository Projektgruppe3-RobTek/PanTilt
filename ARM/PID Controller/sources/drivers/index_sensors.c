#include "index_sensors.h"
#include "../../headers/tm4c123gh6pm.h"


void setup_index_sensors(void)
{
  	SYSCTL_RCGC2_R 		|= SYSCTL_RCGC2_GPIOF;
  	GPIO_PORTF_DIR_R  &= ~(SW1_PIN | SW2_PIN); // Init SW1 and SW2 as input.

  	//Unlock PF0
  	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
  	GPIO_PORTF_CR_R  |= 0x01;

  	GPIO_PORTF_DEN_R |= SW1_PIN | SW2_PIN;

  	GPIO_PORTF_DIR_R   &= ~(SW1_PIN | SW2_PIN);   							// Init SW1 as input.
    GPIO_PORTF_AFSEL_R &= ~(SW1_PIN | SW2_PIN);  								// Disable alt funct
    GPIO_PORTF_PCTL_R  &= ~(GPIO_PCTL_PF4_M | GPIO_PCTL_PF0_M); // Configure as GPIO.
    GPIO_PORTF_AMSEL_R &= ~(SW1_PIN | SW2_PIN);  								// Disable analog functionality
    GPIO_PORTF_PUR_R   |=  (SW1_PIN | SW2_PIN);     						// Enable weak pull-up.
    
}
