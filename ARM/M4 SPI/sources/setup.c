#include "../headers/setup.h"

void setup_gpio()
{
	// Enable the GPIO ports that is used for the on-board buttons, and for the lcd.
	SYSCTL_RCGC2_R 		|= SYSCTL_RCGC2_GPIOF;
	SYSCTL_RCGC2_R 		|= SYSCTL_RCGC2_GPIOC;
 	SYSCTL_RCGC2_R 		|= SYSCTL_RCGC2_GPIOD;
	GPIO_PORTF_DIR_R  &= ~(SW1_PIN | SW2_PIN); // Init SW1 and SW2 as input.
	
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R  |= 0x01;

	// Enable the GPIO pins for digital function.
	GPIO_PORTF_DEN_R |= SW1_PIN | SW2_PIN | LED_RED | LED_GREEN | LED_BLUE;
	
	GPIO_PORTF_DIR_R 	 |= LED_RED | LED_GREEN | LED_BLUE;
	GPIO_PORTF_DIR_R   &= ~(SW1_PIN | SW2_PIN);   							// Init SW1 as input.
	GPIO_PORTF_AFSEL_R &= ~(SW1_PIN | SW2_PIN);  								// Disable alt funct on PF4.
	GPIO_PORTF_PCTL_R  &= ~(GPIO_PCTL_PF4_M | GPIO_PCTL_PF0_M); // Configure PF4 as GPIO.
	GPIO_PORTF_AMSEL_R &= ~(SW1_PIN | SW2_PIN);  								// Disable analog functionality on PF4.
	GPIO_PORTF_PUR_R   |=  (SW1_PIN | SW2_PIN);     						// Enable weak pull-up on SW1.
	
	//Setup LCD pins as output
	GPIO_PORTC_DIR_R |= LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
	GPIO_PORTD_DIR_R |= LCD_E  | LCD_RS | LED_STATUS;
	
	GPIO_PORTC_DEN_R |= LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
	GPIO_PORTD_DEN_R |= LCD_E  | LCD_RS | LED_STATUS;
}
