/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: button_events.c
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: See header.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150217  StefanRvo		Created file.
* 150226	MS 					Fixed syntax.
*****************************************************************************/

/***************************** Include files ********************************/
#include "buttons.h"
#include "../os/system_timers.h"
#include "../os/scheduler.h"
#include "../os/systick.h"

/***************************** Enums ****************************************/

typedef enum {
	NO_EVENT,
	SINGLE_PRESS,
	DOUBLE_PRESS,
	LONG_PRESS,
} event;

typedef enum {
	IDLE,
	FIRST_PUSH,
	SECOND_PUSH,
	LONG_PUSH,
	FIRST_RELEASE,
} button_state;

/*****************************   Structs ************************************/
typedef struct {
	volatile uint32_t *port;
	uint32_t pin;
	button_state state;
	INT8U timer;
} button;

/*****************************  Function defs *******************************/
static bool button_pressed(button *button_s);
static event get_button_event(button *button_s);

/******************************	Variables ***********************************/
	static button sw1_button = {&GPIO_PORTF_DATA_R, SW1_PIN, IDLE, 0xFF};
	static button sw2_button = {&GPIO_PORTF_DATA_R, SW2_PIN, IDLE, 0xFF};
	static INT8U button_event_buffer = 0;


/*****************************   Functions   ********************************/
bool button_pressed(button *button_s)
/**********************************************
* Input : Port, pin.
* Output : Output false if pin is high, true if low.
* Function : Check if button is pressed.
**********************************************/
{
	return !(*(button_s->port) & button_s->pin);
}

event get_button_event(button *button_s)
/**********************************************
* Input : Button struct - see header.
* Output : Button event.
* Function : Check if a button event have occurred and returns the event.
**********************************************/
{
	event button_event = NO_EVENT;


	switch(button_s->state)
	{
		case IDLE:
			if(button_pressed(button_s))
			{
				button_s->state = FIRST_PUSH;
				start_timer(button_s->timer, LONG_PRESS_TIME / TIMEOUT_SYSTICK);
			}
			break;

		case FIRST_PUSH:
			if(!button_pressed(button_s))
			{
				button_s->state = FIRST_RELEASE;
				start_timer(button_s->timer, DOUBLE_PRESS_TIME / TIMEOUT_SYSTICK);
			}
			else if(is_timer_finished(button_s->timer))
			{
				button_s->state = LONG_PUSH;
				button_event 		= LONG_PRESS;
			}
			break;

		case FIRST_RELEASE:
			if(button_pressed(button_s))
			{
				button_s->state = SECOND_PUSH;
				start_timer(button_s->timer, LONG_PRESS_TIME / TIMEOUT_SYSTICK);
			}
			else if(is_timer_finished(button_s->timer))
			{
				button_s->state	= IDLE;
				button_event		= SINGLE_PRESS;
			}
			break;

		case SECOND_PUSH:
			if(!button_pressed(button_s))
			{
				button_s->state = IDLE;
				button_event 		= DOUBLE_PRESS;
			}
			else if(is_timer_finished(button_s->timer))
			{
				button_s->state = LONG_PUSH;
				button_event 		= LONG_PRESS;
			}
			break;

		case LONG_PUSH:
			if(!button_pressed(button_s))
			{
				button_s->state = IDLE;
			}
			break;

		default:
			break;
	}
	return button_event;
}


void collect_button_events(void)
{
	event sw_event = get_button_event(&sw1_button);
	if(sw_event == SINGLE_PRESS)
		sys_ringbuf_uchar_push(button_event_buffer, SW1_SINGLE_PRESS);
	else if(sw_event == DOUBLE_PRESS)
		sys_ringbuf_uchar_push(button_event_buffer, SW1_DOUBLE_PRESS);
	else if(sw_event == LONG_PRESS)
		sys_ringbuf_uchar_push(button_event_buffer, SW1_LONG_PRESS);

	sw_event = get_button_event(&sw2_button);
	if(sw_event == SINGLE_PRESS)
		sys_ringbuf_uchar_push(button_event_buffer, SW2_SINGLE_PRESS);
	else if(sw_event == DOUBLE_PRESS)
		sys_ringbuf_uchar_push(button_event_buffer, SW2_DOUBLE_PRESS);
	else if(sw_event == LONG_PRESS)
		sys_ringbuf_uchar_push(button_event_buffer, SW2_LONG_PRESS);

	wait(BUTTON_CHECK_PERIODE / TIMEOUT_SYSTICK);
}

RBUF_INDEX_TYPE button_data_available(void)
{
	return sys_ringbuf_uchar_size(button_event_buffer);
}

INT8U button_in_char(void)
{
	return sys_ringbuf_uchar_pop(button_event_buffer);
}

void setup_buttons(void)
{ //init pins for SW1 and SW2
	sw1_button.timer = request_timer();
	sw2_button.timer = request_timer();
	button_event_buffer = sys_ringbuf_uchar_request();

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
/****************************** End of module *******************************/
