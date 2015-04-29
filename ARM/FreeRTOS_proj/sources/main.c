
/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: leds.c
*
* PROJECT....: ECP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "status_led.h"
#include "os/system_buffers.h"
#include "drivers/UART.h"
#include "drivers/SSI0.h"
#include "drivers/SSI3.h"
#include "drivers/sysctl.h"
#include "drivers/fpu.h"
#include "tasks/sampler_tasks.h"
#include "tasks/debug_task.h"
#include "libs/print.h"
#include "../headers/GLOBAL_DEFINITIONS.h"
#include "tasks/controller_task.h"
#include "tasks/uart_stuff.h"


/*****************************    Defines    *******************************/
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void uart_task(void *pvParameters);
void spi_task(void *pvParameters);
void spi2_task(void *pvParameters);

static void setupHardware(void);

static void setupHardware(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :
*****************************************************************************/
{
  // TODO: Put hardware configuration and initialisation in here

  // Warning: If you do not initialize the hardware clock, the timings will be inaccurate
  enable_fpu();
  init_sampler1();
  init_sampler2();
  sys_ringbuf_uchar_init();
  setup_uart0();
  setup_ssi0();
  setup_ssi3();
  status_led_init();
  set_sysclk(FCPU / 1000);
}


int main(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :
*****************************************************************************/
{
  portBASE_TYPE return_value = pdTRUE;

  setupHardware();

  /*
   Start the tasks defined within this file/specific to this demo.
   */

  //create the controller tasks
  return_value &= xTaskCreate( controller1_task, (signed portCHAR *) "Controller 1", 200, NULL,HIGH_PRIO, &controller1_handle);
  return_value &= xTaskCreate( controller2_task, (signed portCHAR *) "Controller 1", 200, NULL,HIGH_PRIO, &controller2_handle);
  return_value &= xTaskCreate( sampler1_task, (signed portCHAR *) "Sampler1", 200, NULL, LOW_PRIO,NULL);
  return_value &= xTaskCreate( sampler2_task, (signed portCHAR *) "Sampler2", 200, NULL, LOW_PRIO,NULL);
  //return_value &= xTaskCreate( debug_task, (signed portCHAR *) "debug", 100,NULL,LOW_PRIO,NULL);
  return_value &= xTaskCreate( status_led_task, ( signed portCHAR * ) "Status_led", 200, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( uart_control, ( signed portCHAR * ) "Status_led", 200, NULL, LOW_PRIO, NULL );


  if (return_value != pdTRUE)
  {
    vprintf_(uart0_out_string, 200, "CRASH IN SETUP\n");
    while(1);  // cold not create one or more tasks.
  }
    vprintf_(uart0_out_string, 200, "Board is booted\n");



  /*
   * Start the scheduler.
   */
  vTaskStartScheduler();

  /*
   * Will only get here if there was insufficient memory to create the idle task.
   */
  return 1;
}
