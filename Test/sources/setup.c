/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: setup.c
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: Setup functions.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150212  MS    			Syntax Fixed.
* 150217  StefanRvo		Split setup into multiple functions.
* 150226  MS    			Syntax Fixed.
*****************************************************************************/

/***************************** Include files ********************************/
#include "../headers/setup.h"

/*****************************   Functions   ********************************/

void setup_gpio()
/**********************************************
* Input : None.
* Output : None.
* Function : SW1 and SW2 as input, LCD pins as output.
**********************************************/
{
	// Enable the GPIO ports that is used for the on-board buttons, and for the lcd.
	SYSCTL_RCGC2_R 		|= SYSCTL_RCGC2_GPIOF;
	SYSCTL_RCGC2_R 		|= SYSCTL_RCGC2_GPIOC;
  SYSCTL_RCGC2_R 		|= SYSCTL_RCGC2_GPIOD;
  SYSCTL_RCGC2_R		|=  SYSCTL_RCGC2_GPIOE;
  
  
  __asm__("NOP");
  __asm__("NOP");
  
  GPIO_PORTE_DEN_R |= 1<<1;
  
  GPIO_PORTE_DIR_R &=~(1<<1);
  GPIO_PORTE_AFSEL_R &= ~(1<<1);
  GPIO_PORTE_PCTL_R &= ~0xF0;
  GPIO_PORTE_AMSEL_R &= ~(1<<1);
  GPIO_PORTE_PUR_R |= 1<<1;
  GPIO_PORTE_IS_R &= ~(1<<1);
  GPIO_PORTE_IBE_R &=~(1<<1);
  GPIO_PORTE_IEV_R &= ~(1<<1);
  GPIO_PORTE_ICR_R = 1<<1;
  GPIO_PORTE_IM_R |= 1<<1;
  NVIC_EN0_R |= (0x01 << (INT_GPIOE-16));
   
  
  
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

/****************************** End of module *******************************/

