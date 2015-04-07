#include "index_sensors.h"
#include "../../headers/tm4c123gh6pm.h"



void setup_index_sensors(void)
{
  	SYSCTL_RCGC2_R 		|= SYSCTL_RCGC2_GPIOE;
  	GPIO_PORTE_DIR_R  &= ~(INDEX_0 | INDEX_1); // Init as input.

  	GPIO_PORTE_DEN_R |= INDEX_0 | INDEX_1;

  	GPIO_PORTE_DIR_R   &= ~(INDEX_0 | INDEX_1);   							// Init as input.
    GPIO_PORTE_AFSEL_R &= ~(INDEX_0 | INDEX_1);  								// Disable alt funct
    GPIO_PORTE_PCTL_R  &= ~(GPIO_PCTL_PE1_M | GPIO_PCTL_PE2_M); // Configure as GPIO.
    GPIO_PORTE_AMSEL_R &= ~(INDEX_0 | INDEX_1);  								// Disable analog functionality
    GPIO_PORTE_PUR_R   |=  (INDEX_0 | INDEX_1);     						// Enable weak pull-up.

}
