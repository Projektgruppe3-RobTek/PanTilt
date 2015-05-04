#include "sampler_tasks.h"
#include "task.h"
#include "../drivers/SSI0.h"
#include "../drivers/SSI3.h"
#include "../libs/print.h"
#include "../drivers/UART.h"
#include "controller_task.h"
#include "../drivers/sysctl.h"
#include "../drivers/leds.h"
xQueueHandle sampler1_queue;
xQueueHandle sampler2_queue;
xSemaphoreHandle sampler1_queue_sem;
xSemaphoreHandle sampler2_queue_sem;
xTaskHandle sampler1_handle;
xTaskHandle sampler2_handle;



static INT8S pwm_motor1 = 0;
static INT8S pwm_motor2 = 0;
static INT32S pos1;
static INT32S pos2;
static bool sampler1_rdy = 0;
static bool sampler2_rdy = 0;

static INT32S calc_position(INT32S last_pos, INT8S change);
static void emergency_stop(void);
void timer1_int(void);


void timer1_int()
{
	//in the timer interrupt, send pwm to ssi, and resume sampling tasks.
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;
  if(sampler1_rdy)
  {
    INT16U outdata = (INT8U)pwm_motor1 | 0 << 8 | 0 << 9;
    ssi0_out_16bit(outdata);
    ssi0_out_16bit(1 << 8);
    ssi0_out_16bit(1 << 8);
    xTaskResumeFromISR(sampler1_handle);

  }
  if(sampler2_rdy)
  {
    INT16U outdata = (INT8U)pwm_motor2 | 0 << 8 | 0 << 9;
    ssi3_out_16bit(outdata);
    ssi3_out_16bit(1 << 8);
    ssi3_out_16bit(1 << 8);
    xTaskResumeFromISR(sampler2_handle);
  }

}

INT32S get_position1(void)
{
  return pos1;
}

INT32S get_position2(void)
{
  return pos2;
}


void set_pwm1(INT8S pwm)
{
  pwm_motor1 = pwm;
}

void set_pwm2(INT8S pwm)
{
  pwm_motor2 = pwm;
}


INT32S calc_position(INT32S last_pos, INT8S change)
{
  return last_pos + change;
}



void emergency_stop()
{
  while(1)
  {
    set_pwm1(0);
  }
}

void calibrate_sampler1(void)
{
  bool index_detected = false;
  bool end_detected = false;
  while(!index_detected)
  {
    if(end_detected) set_pwm1(SAMPLER1_CALIB_PWM * 1.5);
    else             set_pwm1(-SAMPLER1_CALIB_PWM);
    while(ssi0_data_available() < 3)
    ;
    ssi0_in_16bit();
    INT32U in_data = ssi0_in_16bit() << 16;
    in_data |= ssi0_in_16bit();
    bool index =        in_data & 0x400000;
    bool end =          in_data & 0x800000;
    if(end) end_detected = true;
    index_detected = index;
    vTaskSuspend(NULL);
  }
  return;
}


void sampler1_task(void __attribute__((unused)) *pvParameters)
{
  //send a reset request
  INT16U outdata = (INT8U)0 | 0 << 8 | 1 << 9;
  ssi0_out_16bit(outdata);
  while(!ssi0_data_available());
  ssi0_in_16bit();
  sampler1_rdy = 1;
  if(sampler1_rdy && sampler2_rdy)
  TIMER1_CTL_R |= TIMER_CTL_TAEN; // Enable TIMER1_A/sampling.

  vTaskSuspend(NULL);
  calibrate_sampler1();
  INT32S last_pos = 0;
  while(1)
  {
    //#ifdef SAMPLE_DEBUG
    //toggle pin to show frequency
    //GPIO_PORTB_DATA_R ^= (1 << 0);
    //#endif

    while(ssi0_data_available() < 3)
    ;
    ssi0_in_16bit();
    INT32U in_data = ssi0_in_16bit() << 16;
    in_data |= ssi0_in_16bit();
    INT32U timer_val =  in_data & 0x3fffff;
    bool index =        in_data & 0x400000;
    bool end =          in_data & 0x800000;
    INT8S encoder_val = (in_data & 0xff000000) >> 24;


    if(index)
      last_pos = 0;
    else
      last_pos = calc_position(last_pos, encoder_val);
    pos1 = last_pos;//set position variable
    if(end) emergency_stop();
    sample_element element;
    element.position = last_pos;
    element.speed = encoder_val; //speed is noramlised against average sampling time
    element.time_delta = timer_val;

    xSemaphoreTake(sampler1_queue_sem, portMAX_DELAY);
    xQueueSendToBack(sampler1_queue, &element, 0);
    xSemaphoreGive(sampler1_queue_sem);

    //resume the controller task
    vTaskResume(controller1_handle);
    vTaskSuspend(NULL);
  }
}

void calibrate_sampler2(void)
{
  bool index_detected = false;
  while(!index_detected)
  {
    set_pwm2(SAMPLER2_CALIB_PWM);
    while(ssi3_data_available() < 3)
    ;
    ssi3_in_16bit();
    INT32U in_data = ssi3_in_16bit() << 16;
    in_data |= ssi3_in_16bit();
    bool index =        in_data & 0x400000;
    index_detected = index;
    vTaskSuspend(NULL);
  }
  return;
}

void sampler2_task(void __attribute__((unused)) *pvParameters)
{
  //send a reset request
  INT16U outdata = (INT8U)0 | 0 << 8 | 1 << 9;
  ssi3_out_16bit(outdata);
  while(!ssi3_data_available());
  ssi3_in_16bit();

  sampler2_rdy = 1;
  TIMER1_CTL_R |= TIMER_CTL_TAEN; // Enable TIMER1_A/sampling.
  vTaskSuspend(NULL);

  calibrate_sampler2();
  INT32S last_pos = 0;
  while(1)
  {
    while(ssi3_data_available() < 3)
    ;
    ssi3_in_16bit();
    INT32U in_data = ssi3_in_16bit() << 16;
    in_data |= ssi3_in_16bit();
    INT32U timer_val =  in_data & 0x3fffff;
    bool index =        in_data & 0x400000;
    bool reset_but =          in_data & 0x800000;
    INT8S encoder_val = (in_data & 0xff000000) >> 24;

    if(reset_but)
    {
      do_reset();
    }
    if(index)
      last_pos = 0;
    else
      last_pos = calc_position(last_pos, encoder_val);
    pos2 = last_pos;//set position variable

    sample_element element;
    element.position = last_pos;
    element.speed = encoder_val; //speed is noramlised against average sampling time
    element.time_delta = timer_val;

    xSemaphoreTake(sampler2_queue_sem, portMAX_DELAY);
    xQueueSendToBack(sampler2_queue, &element, 0);
    xSemaphoreGive(sampler2_queue_sem);

    vTaskResume(controller2_handle);
    vTaskSuspend(NULL); //suspend. Will be woken up by timer interrupt
  }
}



void init_sampler1()
{ //setup the queues and semaphores
	sampler1_queue =  xQueueCreate(128, sizeof(sample_element));
  vSemaphoreCreateBinary(sampler1_queue_sem);
  //#ifdef SAMPLE_DEBUG
  //setup a pin for debugging purposes, use PB2.
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;

  __asm__("NOP");

  GPIO_PORTB_DIR_R |= 0x04 | 0x01;
  GPIO_PORTB_DEN_R |= 0x04 | 0x01;
  //#endif
}

void init_sampler2()
{ //setup the queues and semaphores
	sampler2_queue =  xQueueCreate(128, sizeof(sample_element));
  vSemaphoreCreateBinary(sampler2_queue_sem);
}
