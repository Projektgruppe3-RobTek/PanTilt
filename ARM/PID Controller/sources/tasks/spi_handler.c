#include "spi_handler.h"
#include "../drivers/leds.h"
#include "../drivers/index_sensors.h"
#include "../drivers/SSI0.h"
#include "../drivers/SSI3.h"

static double speed = 0;
static IN16U  motor_pos = 0;
void spi_handler(void)
{
  if(INDEX_PORT & INDEX_0) LED_RGB_PORT |= LED_RED;
  else LED_RGB_PORT &= LED_RED;

  if(INDEX_PORT & INDEX_1) LED_RGB_PORT |= LED_BLUE;
  else LED_RGB_PORT &= LED_BLUE;
}

void reset_pan_tilt(void)
{
  bool pased_index0 = false;
  bool pased_index1 = false;
  while(!pased_index0 || !pased_index1)
  {
    if(!pased_index0)
    {
      ssi0_in_clear();
      if(INDEX_PORT & INDEX_0) pased_index0 = true;
    }
    if(!pased_index1)
    {
      ssi3_in_clear();
      if(INDEX_PORT & INDEX_1) pased_index1 = true;
    }
  }
}

double get_speed()
{
  return speed;
}
INT16U get_motor_pos()
{
  return 
}
