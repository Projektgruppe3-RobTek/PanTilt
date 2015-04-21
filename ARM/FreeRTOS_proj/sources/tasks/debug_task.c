#include "../drivers/UART.h"
#include "../libs/print.h"
#include "sampler_tasks.h"
#include "task.h"
#include "debug_task.h"
void debug_task(void __attribute__((unused)) *pvParameters)
{
  double deb_pos;
  double deb_speed;
  while(1)
  {
    bool gotsamples = false;
    sample_element sample;
    xSemaphoreTake(sampler1_queue_sem, portMAX_DELAY);
    if(uxQueueMessagesWaiting(sampler1_queue) >= 2)
    {
      gotsamples = true;
      for(INT8U i = 0; i < 2; i++)
      {
        xQueueReceive(sampler1_queue, &sample, 0);
        if(sample.type == position)
        {
          deb_pos = sample.value;
        }
        else if(sample.type == speed)
        {
          deb_speed = sample.value;
        }
      }
    }
    xSemaphoreGive(sampler1_queue_sem);
    if(gotsamples)
      vprintf_(uart0_out_string, 200, "POSITION: %d, SPEED %d\n", (int)deb_pos, (int)deb_speed);
    vTaskDelay(100 / portTICK_RATE_NS);
  }
}
