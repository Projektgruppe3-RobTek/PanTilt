#include "time.h"
#include <stdlib.h>

void time_count_sec(time *time_s, INT32S count)
{
  bool direction = (count >= 0);

  if(abs(count) >= SECONDS_PER_MINUTE * MINUTES_PER_HOUR * HOURS_PER_DAY)
  {
    //Just take modulus to remove extra days
    count %= SECONDS_PER_MINUTE * MINUTES_PER_HOUR * HOURS_PER_DAY;
  }
  if(abs(count) >= SECONDS_PER_MINUTE * MINUTES_PER_HOUR)
  {
    time_count_hour(time_s, count / (SECONDS_PER_MINUTE * MINUTES_PER_HOUR));
    count %= SECONDS_PER_MINUTE * MINUTES_PER_HOUR;
  }
  if(abs(count) >= SECONDS_PER_MINUTE)
  {
    time_count_min(time_s, count / SECONDS_PER_MINUTE);
    count %= SECONDS_PER_MINUTE;
  }


  if(direction)
  {
  	if(time_s->sec + count >= SECONDS_PER_MINUTE)
  	{
  		time_s->sec = (time_s->sec + count) % SECONDS_PER_MINUTE;
      time_count_min(time_s, 1);
    }
    else
    {
      time_s->sec += count;
    }
  }
  else
  {
  	if( ((INT8S)time_s->sec) + count < 0)
    {
      time_s->sec = ((INT8S)time_s->sec + count + SECONDS_PER_MINUTE) % SECONDS_PER_MINUTE;
      time_count_min(time_s, -1);
    }
    else
    {
      time_s->sec += count;
    }
  }
}

void time_count_min(time *time_s, INT32S count)
{
  bool direction = (count >= 0);
  if(abs(count) >= MINUTES_PER_HOUR * HOURS_PER_DAY)
  {
    count %= MINUTES_PER_HOUR * HOURS_PER_DAY;
  }
  if(abs(count) >= HOURS_PER_DAY)
  {
    time_count_hour(time_s, count / MINUTES_PER_HOUR);
    count %= MINUTES_PER_HOUR;
  }

  if(direction)
  {
  	if(time_s->min + count >= MINUTES_PER_HOUR)
  	{
  		time_s->min = (time_s->min + count) % MINUTES_PER_HOUR;
      time_count_hour(time_s, 1);
    }
    else
    {
      time_s->min += count;
    }
  }
  else
  {
  	if( ((INT8S)time_s->min) + count < 0)
    {
      time_s->min = ((INT8S)time_s->min + count+ MINUTES_PER_HOUR) % MINUTES_PER_HOUR;
      time_count_hour(time_s, -1);
    }
    else
    {
      time_s->min += count;
    }
  }
}


void time_count_hour(time *time_s, INT32S count)
{
  if(abs(count) > HOURS_PER_DAY)
  {
    count %= HOURS_PER_DAY;
  }

	time_s->hour = (time_s->hour + count + HOURS_PER_DAY) % HOURS_PER_DAY;
}
