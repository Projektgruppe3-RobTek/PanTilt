#include "sampler_tasks.h"
#include "task.h"
#include "../drivers/SSI0.h"
#include "../drivers/SSI3.h"
#include "../libs/print.h"
#include "../drivers/UART.h"
#include "controller_task.h"
#include "../drivers/sysctl.h"
xQueueHandle sampler1_queue;
xQueueHandle sampler2_queue;
xSemaphoreHandle sampler1_queue_sem;
xSemaphoreHandle sampler2_queue_sem;
INT8S pwm_motor1 = 0;
INT8S pwm_motor2 = 0;
bool reset = 0;

static INT32S calc_position(INT32S last_pos, INT8S change);
static double calc_speed(INT32U time, INT8S encode_pulses);
static void emergency_stop(void);
static void do_reset_stuff(void);




INT32S calc_position(INT32S last_pos, INT8S change)
{
  return last_pos + change;
}

double calc_speed(INT32U __attribute__((unused)) time, INT8S encode_pulses)
{
  return encode_pulses;
}

void emergency_stop()
{
  while(1)
  {
    INT16U outdata = (INT8U)0 | 0 << 8 | reset << 9;
    ssi0_out_16bit(outdata);
    ssi0_out_16bit(1 << 8);
    ssi0_out_16bit(1 << 8);
  }
}

void do_reset_stuff()
{
  //just reset the arm board for now.
  do_reset();
}

void calibrate_sampler1(void)
{
  portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  bool index_detected = false;
  bool end_detected = false;
  while(!index_detected)
  {
    INT8S pwm_speed = 0;
    if(end_detected) pwm_speed =  SAMPLER1_CALIB_PWM * 1.5;
    else             pwm_speed =  -SAMPLER1_CALIB_PWM;
    INT16U outdata = (INT8U)pwm_speed | 0 << 8 | reset << 9;
    ssi0_out_16bit(outdata);
    ssi0_out_16bit(1 << 8);
    ssi0_out_16bit(1 << 8);
    while(ssi0_data_available() < 3)
    ;
    ssi0_in_16bit();
    INT32U in_data = ssi0_in_16bit() << 16;
    in_data |= ssi0_in_16bit();
    bool index =        in_data & 0x400000;
    bool end =          in_data & 0x800000;
    if(end) end_detected = true;
    index_detected = index;
    vTaskDelayUntil(&xLastWakeTime, SAMPLE_TIME / portTICK_RATE_NS );
  }
  return;
}


void sampler1_task(void __attribute__((unused)) *pvParameters)
{
  calibrate_sampler1();
  INT32S last_pos = 0;
  portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
    INT16U outdata = (INT8U)pwm_motor1 | 0 << 8 | reset << 9;
    ssi0_out_16bit(outdata);
    ssi0_out_16bit(1 << 8);
    ssi0_out_16bit(1 << 8);

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
    if(end) emergency_stop();
    sample_element element;
    element.position = last_pos;
    element.speed = calc_speed(timer_val, encoder_val);
    element.time_delta = timer_val;

    xSemaphoreTake(sampler1_queue_sem, portMAX_DELAY);
    xQueueSendToBack(sampler1_queue, &element, 0);
    xSemaphoreGive(sampler1_queue_sem);

    //resume the controller task
    vTaskResume(controller1_handle);
    vTaskDelayUntil(&xLastWakeTime, SAMPLE_TIME / portTICK_RATE_NS );
  }
}

void calibrate_sampler2(void)
{
  portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  bool index_detected = false;
  while(!index_detected)
  {
    INT8S pwm_speed = SAMPLER2_CALIB_PWM;
    INT16U outdata = (INT8U)pwm_speed | 0 << 8 | reset << 9;
    ssi3_out_16bit(outdata);
    ssi3_out_16bit(1 << 8);
    ssi3_out_16bit(1 << 8);
    while(ssi3_data_available() < 3)
    ;
    ssi3_in_16bit();
    INT32U in_data = ssi3_in_16bit() << 16;
    in_data |= ssi3_in_16bit();
    bool index =        in_data & 0x400000;
    index_detected = index;
    vTaskDelayUntil(&xLastWakeTime, SAMPLE_TIME / portTICK_RATE_NS );
  }
  return;
}

void sampler2_task(void __attribute__((unused)) *pvParameters)
{
  calibrate_sampler2();
  INT32S last_pos = 0;
  portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
    INT16U outdata = (INT8U)pwm_motor2 | 0 << 8 | reset << 9;
    ssi3_out_16bit(outdata);
    ssi3_out_16bit(1 << 8);
    ssi3_out_16bit(1 << 8);

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
      do_reset_stuff();
    }
    if(index)
      last_pos = 0;
    else
      last_pos = calc_position(last_pos, encoder_val);

    sample_element element;
    element.position = last_pos;
    element.speed = calc_speed(timer_val, encoder_val);
    element.time_delta = timer_val;

    xSemaphoreTake(sampler2_queue_sem, portMAX_DELAY);
    xQueueSendToBack(sampler2_queue, &element, 0);
    xSemaphoreGive(sampler2_queue_sem);
    vTaskResume(controller2_handle);
    vTaskDelayUntil(&xLastWakeTime, SAMPLE_TIME / portTICK_RATE_NS );
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
