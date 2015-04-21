#include "sampler_tasks.h"
#include "task.h"
#include "../drivers/SSI0.h"
#include "../drivers/SSI3.h"
xQueueHandle sampler1_queue;
xQueueHandle sampler2_queue;
xSemaphoreHandle sampler1_queue_sem;
xSemaphoreHandle sampler2_queue_sem;
INT8S pwm_motor1;
INT8S pwm_motor2;
bool reset;

static INT32S calc_position(INT32S last_pos, INT8S change);
static double calc_speed(INT32U time, INT8S encode_pulses);
static void emergency_stop(void);
static void do_reset_stuff(void);




INT32S calc_position(INT32S last_pos, INT8S change)
{
  return last_pos + change;
}
double calc_speed(INT32U time, INT8S encode_pulses)
{
  double pulses_per_80ns = (double)encode_pulses/(double)time;
  double pulses_per_ms =  pulses_per_80ns * 1000000 / 80;
  double pulses_per_second =  pulses_per_ms * 1000;
  return pulses_per_second;
}

void emergency_stop()
{
  while(1);
}
void do_reset_stuff()
{

}

void sampler1_task(void __attribute__((unused)) *pvParameters)
{
  INT32S last_pos = 0;
  portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
    INT16U outdata = pwm_motor1 | 0 << 8 | reset << 9;
    ssi0_out_16bit(outdata);
    ssi0_out_16bit(1 << 8);
    ssi0_out_16bit(1 << 8);

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
    sample_element position_element;
    position_element.type = position;
    position_element.value = last_pos;
    sample_element speed_element;
    speed_element.type = speed;
    speed_element.value = calc_speed(timer_val, encoder_val);

    xSemaphoreTake(sampler1_queue_sem, portMAX_DELAY);
    xQueueSendToBack(sampler1_queue, &position_element, 0);
    xQueueSendToBack(sampler1_queue, &speed_element, 0);
    xSemaphoreGive(sampler1_queue_sem);

    vTaskDelayUntil(&xLastWakeTime, TICK_RATE / SAMPLE_FREQ );
  }
}

void sampler2_task(void __attribute__((unused)) *pvParameters)
{
  INT32S last_pos = 0;
  portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
    INT16U outdata = pwm_motor2 | 0 << 8 | reset << 9;
    ssi3_out_16bit(outdata);
    ssi3_out_16bit(1 << 8);
    ssi3_out_16bit(1 << 8);

    while(ssi3_data_available() < 3)
    ;
    ssi3_in_16bit();
    INT32U in_data = ssi3_in_16bit() << 16;
    in_data |= ssi3_in_16bit();
    INT32U timer_val =        in_data & 0x3fffff;
    bool index =              in_data & 0x400000;
    bool reset_all =          in_data & 0x800000;
    INT8S encoder_val =       (in_data & 0xff000000) >> 24;

    if(index)
      last_pos = 0;
    else
      last_pos = calc_position(last_pos, encoder_val);
    if(reset_all) do_reset_stuff();
    sample_element position_element;
    position_element.type = position;
    position_element.value = last_pos;
    sample_element speed_element;
    speed_element.type = speed;
    speed_element.value = calc_speed(timer_val, encoder_val);

    xSemaphoreTake(sampler2_queue_sem, portMAX_DELAY);
    xQueueSendToBack(sampler2_queue, &position_element, 0);
    xQueueSendToBack(sampler2_queue, &speed_element, 0);
    xSemaphoreGive(sampler2_queue_sem);

    vTaskDelayUntil(&xLastWakeTime, TICK_RATE / SAMPLE_FREQ );
  }
}

void init_sampler1()
{ //setup the queues and semaphores
	sampler1_queue =  xQueueCreate(256, sizeof(sample_element));
  vSemaphoreCreateBinary(sampler1_queue_sem);
}

void init_sampler2()
{ //setup the queues and semaphores
	sampler2_queue =  xQueueCreate(256, sizeof(sample_element));
  vSemaphoreCreateBinary(sampler2_queue_sem);
}
