#include "../drivers/UART.h"
#include "../libs/print.h"
#include "sampler_tasks.h"
#include "task.h"
#include "debug_task.h"
void debug_task(void __attribute__((unused)) *pvParameters)
{
  double deb_pos = 0;
  double deb_speed = 0;
  while(1)
  {
    bool gotsamples = false;
    sample_element sample;
    xSemaphoreTake(sampler2_queue_sem, portMAX_DELAY);
    if(uxQueueMessagesWaiting(sampler2_queue) >= 1)
    {
      gotsamples = true;
      xQueueReceive(sampler2_queue, &sample, 0);
      deb_pos = sample.position;
      deb_speed = sample.speed;
    }
    xSemaphoreGive(sampler2_queue_sem);
    if(gotsamples)
      vprintf_(uart0_out_string, 200, "POSITION: %d, SPEED %d\n", (int)deb_pos, (int)deb_speed);
    vTaskDelay(100000 / portTICK_RATE_US);
  }
}
