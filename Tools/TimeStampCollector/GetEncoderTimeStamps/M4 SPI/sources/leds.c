#include "../headers/leds.h"

void set_leds(INT8U counter_val)
{
	// Turn off LEDs.
	GPIO_PORTF_DATA_R &= ~(LED_RED | LED_GREEN | LED_BLUE);

	// Set LEDs.
	switch (counter_val)
	{
		case 1:
			GPIO_PORTF_DATA_R |= LED_GREEN;
			break;
		case 2:
			GPIO_PORTF_DATA_R |= LED_BLUE;
			break;
		case 3:
			GPIO_PORTF_DATA_R |= LED_GREEN | LED_BLUE;
			break;
		case 4:
			GPIO_PORTF_DATA_R |= LED_RED;
			break;
		case 5:
			GPIO_PORTF_DATA_R |= LED_RED | LED_GREEN;
			break;
		case 6:
			GPIO_PORTF_DATA_R |= LED_RED | LED_BLUE;
			break;
		case 7:
			GPIO_PORTF_DATA_R |= LED_RED | LED_GREEN | LED_BLUE;
			break;
		default :
			break;
	}
}
