#include "spi_handler.h"
#include "../drivers/leds.h"
#include "../drivers/index_sensors.h"
void spi_handler(void)
{
  if(INDEX_PORT & INDEX_0) LED_RGB_PORT |= LED_RED;
  else LED_RGB_PORT &= LED_RED;

  if(INDEX_PORT & INDEX_1) LED_RGB_PORT |= LED_BLUE;
  else LED_RGB_PORT &= LED_BLUE;
}
