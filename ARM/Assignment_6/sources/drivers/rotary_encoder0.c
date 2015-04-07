#include "rotary_encoder0.h"
#include <stdbool.h>

static INT32S encoder0_pos = 0;
static INT8U lastval;
static INT32U encoder_pushes = 0;

INT32S get_encoder0_pos(void)
{
  return encoder0_pos;
}

void reset_encoder0(void)
{
  encoder0_pos = 0;
}

void scan_encoder0(void)
{
  static bool enc_button_last = 1;
  //Read port status
  INT8U tmp = GPIO_PORTA_DATA_R & 0xFF;
  bool enc_a = tmp & DIGI_A;
  bool enc_b = tmp & DIGI_B;
  bool enc_switch = tmp & DIGI_P2;
  INT8U curval = (enc_a << 1) + enc_b;
  if(!enc_switch && enc_button_last != enc_switch)
  { //Don't do debouncing for now.
    encoder_pushes++;
  }

  else if(lastval !=  curval)
  {  //If last values LSB xor this values MSB is 1, we are going CW, else we are going CCW.
    if ( (lastval & 0x1) ^ (curval >> 1) )
      encoder0_pos++;
    else
      encoder0_pos--;
  }

  lastval = curval;
  enc_button_last = enc_switch;
}

void encoder0_consume_push()
{ //Count encoder_pushes down.
  if(encoder_pushes)
  {
     encoder_pushes--;
  }
}
bool encoder0_pushed() //Return whether the encoder button has ben pushed since last request.
{
  if(encoder_pushes)
    return true;
  else
    return false;
}

void setup_encoder0(void)
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // activate port A

  __asm__("NOP");
  //Setup pins as input
  GPIO_PORTA_DIR_R 	 &= ~ (DIGI_A | DIGI_B | DIGI_P2);
  GPIO_PORTA_DEN_R 	 |=   (DIGI_A | DIGI_B | DIGI_P2); 	//Digital enable
  GPIO_PORTA_AFSEL_R &= ~ (DIGI_A | DIGI_B | DIGI_P2);	//Disable alt func.
  GPIO_PORTA_PCTL_R  &= ~ (GPIO_PCTL_PA5_M | GPIO_PCTL_PA6_M | GPIO_PCTL_PA7_M); //Configure as GPIO.
  GPIO_PORTA_AMSEL_R &= ~ (DIGI_A | DIGI_B | DIGI_P2);  //Disable analog functionality.

  //Reset counter and read current value in.
  reset_encoder0();
  INT8U tmp = GPIO_PORTA_DATA_R & 0xFF;
  bool enc_a = tmp & DIGI_A;
  bool enc_b = tmp & DIGI_B;
  lastval = (enc_a << 1) + enc_b;
}
