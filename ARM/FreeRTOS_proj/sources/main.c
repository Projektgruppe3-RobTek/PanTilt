
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
#include "glob_def.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "systick.h"
#include "status_led.h"
#include "os/system_buffers.h"
#include "drivers/UART.h"
#include "drivers/SSI0.h"
#include "drivers/SSI3.h"

/*****************************    Defines    *******************************/
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/


static void setupHardware(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :
*****************************************************************************/
{
  // TODO: Put hardware configuration and initialisation in here

  // Warning: If you do not initialize the hardware clock, the timings will be inaccurate
  init_systick();
  sys_ringbuf_uchar_init();
  setup_uart0();
  setup_ssi0();
  setup_ssi3();
  status_led_init();

}
void uart_task(void *pvParameters)
{
  while(1)
  {
    uart0_out_char('t');
  	vTaskDelay(100 / portTICK_RATE_MS); // wait 100 ms.
  }
}

void spi_task(void *pvParameters)
{
  while(1)
  {
    ssi0_out_16bit(0b0000000011011000);
    ssi0_out_16bit(0b0000000100000000);
    ssi0_out_16bit(0b0000000100000000);

    ssi0_in_16bit();
    INT32U in_data = ssi0_in_16bit() << 16;
    in_data |= ssi0_in_16bit();
    bool index_bit = in_data & 0x400000;
    if(index_bit)
    {
      ssi0_out_16bit(0b0000000000000000);
      while(1)
        vTaskDelay(1000 / portTICK_RATE_MS); // wait 100 ms.
    }
    //uart0_out_char((in_data >> 24) & 0xFF);
    //uart0_out_char((in_data >> 16) & 0xFF);
    //uart0_out_char((in_data >> 8) & 0xFF);
    //uart0_out_char((in_data     ) & 0xFF);

    vTaskDelay(1 / portTICK_RATE_MS); // wait 100 ms.
  }
}

void spi2_task(void *pvParameters)
{
  while(1)
  {
    ssi3_out_16bit(0b0000000001001000);
    ssi3_out_16bit(0b0000000100000000);
    ssi3_out_16bit(0b0000000100000000);

    ssi3_in_16bit();
    INT32U in_data = ssi3_in_16bit() << 16;
    in_data |= ssi3_in_16bit();
    bool index_bit = in_data & 0x400000;
    if(index_bit)
    {
      ssi3_out_16bit(0b0000000000000000);
      while(1)
        vTaskDelay(1000 / portTICK_RATE_MS); // wait 100 ms.
    }
    //uart0_out_char((in_data >> 24) & 0xFF);
    //uart0_out_char((in_data >> 16) & 0xFF);
    //uart0_out_char((in_data >> 8) & 0xFF);
    //uart0_out_char((in_data     ) & 0xFF);

    vTaskDelay(1 / portTICK_RATE_MS); // wait 100 ms.
  }
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
  return_value &= xTaskCreate( status_led_task, ( signed portCHAR * ) "Status_led", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( spi_task, (signed portCHAR *) "Uart", USERTASK_STACK_SIZE,NULL,LOW_PRIO,NULL);
  return_value &= xTaskCreate( spi2_task, (signed portCHAR *) "Uart", USERTASK_STACK_SIZE,NULL,LOW_PRIO,NULL);

  if (return_value != pdTRUE)
  {
    GPIO_PORTD_DATA_R &= 0xBF;  // Turn on status LED.
    while(1);  // cold not create one or more tasks.
  }


  /*
   * Start the scheduler.
   */
  vTaskStartScheduler();

  /*
   * Will only get here if there was insufficient memory to create the idle task.
   */
  return 1;
}
