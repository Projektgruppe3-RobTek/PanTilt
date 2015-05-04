#include "leds.h"
#include "../../headers/tm4c123gh6pm.h"
void setup_leds(void)
{
	// Enable the GPIO ports that is used for the led's.
	SYSCTL_RCGC2_R 		|= SYSCTL_RCGC2_GPIOF;
  SYSCTL_RCGC2_R 		|= SYSCTL_RCGC2_GPIOD;

	__asm__("NOP");
	__asm__("NOP");

	// Enable the GPIO pins for digital function.
	GPIO_PORTF_DEN_R |= LED_RED | LED_GREEN | LED_YELLOW;
	GPIO_PORTF_DIR_R |= LED_RED | LED_GREEN | LED_YELLOW;

  //Setup LCD pins as output
  GPIO_PORTD_DIR_R |= LED_STATUS;
  GPIO_PORTD_DEN_R |= LED_STATUS;

	LED_RGB_PORT |= LED_RED | LED_GREEN | LED_YELLOW;
}
