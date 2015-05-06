#include "controller_task.h"
#include <stddef.h>
#include "../../headers/tm4c123gh6pm.h"
#include "sampler_tasks.h"
#include "../libs/PID.h"
#include "../libs/print.h"
#include "../drivers/UART.h"
#include "../drivers/leds.h"

#define MAX_OUTPUT_OUTER_1 (330/20.)

xTaskHandle controller1_handle;
xTaskHandle controller2_handle;
static INT16S goal1 = 0;
static INT16S goal2 = 0;

void set_goal1(INT16S goal)
{
  goal1 = goal;
}
void set_goal2(INT16S goal)
{
  goal2 = goal;
}

void controller1_task(void __attribute__((unused)) *pvParameters)
{
  sample_element sample;

  PID_s PID_inner = init_PID(0, 0, 0, 1000); //fast
  PID_s PID_outer = init_PID(0, 0, 0.000, 100); //slow

  vTaskSuspend(NULL);

  while(1)
  { //run outer PID
    //use peak, us we need to use the sample in the forloop
    xSemaphoreTake(sampler1_queue_sem, portMAX_DELAY);
    xQueuePeek(sampler1_queue, &sample, 0);
	   //GPIO_PORTB_DATA_R ^= (1 << 0);

    double position_error = goal1 - sample.position;
    double PID_speed = PID(&PID_outer, position_error);

    xSemaphoreGive(sampler1_queue_sem);
    //vprintf_(uart0_out_string, 200, "%d\n", (int)sample.position);

    for(INT8U i = 0; i < 10; i++)
    { //run inner PID
      xSemaphoreTake(sampler1_queue_sem, portMAX_DELAY);
      while(xQueueReceive(sampler1_queue, &sample, 0))
      {
        //GPIO_PORTB_DATA_R ^= (1 << 3);
        double speed_error = PID_speed - sample.speed;
        double PID_output = PID(&PID_inner, speed_error);
        //pwm output calculation.
        INT8S wanted_pwm = PID_output * 127;
        if(PID_output > 1) wanted_pwm = 127;
        else if(PID_output < -1) wanted_pwm = -128;
        //put out pwm value
        set_pwm1(wanted_pwm);
      }
      xSemaphoreGive(sampler1_queue_sem);
      vTaskSuspend(NULL); //the task is resumed from the sampler
    }

  }

}


void controller2_task(void __attribute__((unused)) *pvParameters)
{
  sample_element sample;

  PID_s PID_inner = init_PID(0.05, 0.000, 0.000, 1000); //fast
  PID_s PID_outer = init_PID(1.6, 4.2, 0.05, 100); //slow
  vTaskSuspend(NULL);

  while(1)
  { //run outer PID
    //use peak, us we need to use the sample in the forloop
    xSemaphoreTake(sampler2_queue_sem, portMAX_DELAY);
    xQueuePeek(sampler2_queue, &sample, 0);

    double position_error = goal2 - sample.position;
    double PID_speed = PID(&PID_outer, position_error);

    if(fabs(PID_speed) > MAX_OUTPUT_OUTER_1)
      PID_speed = (PID_speed/fabs(PID_speed)) * MAX_OUTPUT_OUTER_1;

    xSemaphoreGive(sampler2_queue_sem);

    for(INT8U i = 0; i < 10; i++)
    { //run inner PID
      xSemaphoreTake(sampler2_queue_sem, portMAX_DELAY);
      while(xQueueReceive(sampler2_queue, &sample, 0))
      {
        double speed_error = PID_speed - sample.speed;
        double PID_output = PID(&PID_inner, speed_error);

        //pwm output calculation.
        INT8S wanted_pwm = PID_output * 127;
        if(PID_output > 1) wanted_pwm = 127;
        else if(PID_output < -1) wanted_pwm = -128;
        //put out pwm value
        set_pwm2(wanted_pwm);
      }
      xSemaphoreGive(sampler2_queue_sem);
      vTaskSuspend(NULL); //the task is resumed from the sampler
    }

  }
}
