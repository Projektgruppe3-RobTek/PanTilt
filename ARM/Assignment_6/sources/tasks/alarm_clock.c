#include "alarm_clock.h"
#include "../os/scheduler.h"
#include "../drivers/rotary_encoder0.h"
#include "../drivers/lcd0.h"
#include "../libs/time.h"
#include "../../headers/emp_type.h"
#include "clock.h"
#include "../os/systick.h"
#include "../drivers/leds.h"
#include <stdlib.h>

static void alarm_release_lcd(void);
static void display_alarm_time(time *alarm_time);

static void display_alarm_time(time *alarm_time)
{
  lcd0_set_cursor(0,0);
  lcd0_write_char(alarm_time->hour / 10 + '0');
  lcd0_write_char(alarm_time->hour % 10 + '0');
  lcd0_write_char(':');
  lcd0_write_char(alarm_time->min / 10 + '0');
  lcd0_write_char(alarm_time->min % 10 + '0');
}
static void alarm_release_lcd(void)
{
  lcd0_no_blink();
  lcd0_set_cursor(0,0);
  time cur_time = get_clock();
  lcd0_write_char(cur_time.hour / 10 + '0');
  lcd0_write_char(cur_time.hour % 10 + '0');
  lcd0_write_char(':');
  lcd0_write_char(cur_time.min / 10 + '0');
  lcd0_write_char(cur_time.min % 10 + '0');
  lcd0_write_string("           ");
  lcd0_set_cursor(0,1);
  lcd0_write_string("                ");
  lcd0_unlock();
}

typedef enum
{
  ALARM_NOT_SET,
  SET_ALARM_START,
  ALARM_MENU,
  SET_ALARM,
  ALARM_SET,
  ALARM_ACTIVE_1,
  ALARM_ACTIVE_2
} alarm_state;

void alarm_task(void)
{
  static alarm_state state = ALARM_NOT_SET;
  static INT32S last_encoder_pos = 0;
  static bool menu_selected = 0;
  static time alarm_time = {0,0,0};
  INT32S cur_pos;
  switch(state)
  {
    case ALARM_NOT_SET:
      if (check_release(encoder0_pushed))
      {
        encoder0_consume_push(); // consume push.
        state = SET_ALARM_START;
      }
      else
        break;
    case SET_ALARM_START:
      if(check_release(lcd0_available))
      {
        lcd0_lock();
        //clear display
        lcd0_set_cursor(0,0);
        lcd0_write_string("                ");
        lcd0_set_cursor(0,1);
        lcd0_write_string("                ");
        lcd0_no_blink();
        lcd0_set_cursor(0,0);
        lcd0_write_string("SET ALARM      X");
        lcd0_set_cursor(0,1);
        lcd0_write_string("DISABLE ALARM  X");
        lcd0_blink();
        lcd0_set_cursor(15,0);
        last_encoder_pos = get_encoder0_pos();
        menu_selected = 0;
        state = ALARM_MENU;
      }
      else
        break;
    case ALARM_MENU: //display a menu where you can chose between setting the alarm or not.
      cur_pos = get_encoder0_pos();

      if (abs(last_encoder_pos - cur_pos) >=2 )
      {
        last_encoder_pos = cur_pos;
        menu_selected ^= 1;
        lcd0_set_cursor(15,menu_selected);
      }
      if(encoder0_pushed())
      {
        encoder0_consume_push();
        if(menu_selected)
        {
          alarm_release_lcd();
          state = ALARM_NOT_SET;
        }
        else
        {
          lcd0_no_blink();
          lcd0_set_cursor(0,0);
          lcd0_write_string("                ");
          lcd0_set_cursor(0,1);
          lcd0_write_string("SET THE ALARM   ");
          display_alarm_time(&alarm_time);
          state = SET_ALARM;
        }
      }
    break;

    case SET_ALARM:
      cur_pos = get_encoder0_pos();
      if (last_encoder_pos != cur_pos)
      {
        time_count_min(&alarm_time, (cur_pos - last_encoder_pos));
        last_encoder_pos = cur_pos;
        display_alarm_time(&alarm_time);
      }
      if(encoder0_pushed())
      {
        encoder0_consume_push();
        alarm_release_lcd();
        state = ALARM_SET;
      }
    break;
    case ALARM_SET:
      if(encoder0_pushed())
      {
        encoder0_consume_push();
        state = SET_ALARM_START;
      }
      else
      {
        time cur_time = get_clock();
        if(cur_time.hour == alarm_time.hour && cur_time.min == alarm_time.min)
        {
          state = ALARM_ACTIVE_1;
        }
      }
    break;

    case ALARM_ACTIVE_1:
      if(check_release(lcd0_available))
      {
        if(encoder0_pushed())
        {
          encoder0_consume_push();
          state = ALARM_NOT_SET;
        }
        else
        {
          lcd0_lock();
          lcd0_set_cursor(0,1);
          lcd0_write_string("ALARM");
          lcd0_unlock();
          LED_RGB_PORT |= LED_RED | LED_GREEN | LED_BLUE;
          wait(200 / TIMEOUT_SYSTICK); //wait 200 ms
          state = ALARM_ACTIVE_2;
        }
      }
    break;
    case ALARM_ACTIVE_2:
      if(check_release(lcd0_available))
      {
        lcd0_lock();
        lcd0_set_cursor(0,1);
        lcd0_write_string("     ");
        lcd0_unlock();
        LED_RGB_PORT &= ~(LED_RED | LED_GREEN | LED_BLUE);
        wait(200 / TIMEOUT_SYSTICK); //wait 200 ms
        state = ALARM_ACTIVE_1;
      }
    break;
  }

}
