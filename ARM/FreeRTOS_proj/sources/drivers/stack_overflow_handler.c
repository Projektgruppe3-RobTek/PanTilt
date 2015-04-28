#include "FreeRTOS.h"
#include "task.h"
#include "../libs/print.h"
#include "UART.h"
extern void vApplicationStackOverflowHook( xTaskHandle xTask, signed char *pcTaskName );

void vApplicationStackOverflowHook( xTaskHandle __attribute__((unused)) xTask, signed char *pcTaskName )
{
  vprintf_(uart0_out_string, 200, "Stackoverflow in task %s", pcTaskName);
  while(1);
}
