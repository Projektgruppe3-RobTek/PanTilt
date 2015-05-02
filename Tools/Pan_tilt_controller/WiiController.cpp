#include "WiiController.h"
#include <cmath>
static void poll_loop_dummy(WiiController *obj);

WiiController::WiiController()
{
  std::cout << "Searching for wiimotes..." << std::endl;
  int found = 0;
  while(!wii.GetWiimotes().size())
  {
    found = wii.Find(1);
    std::cout << "Found " << found << " WiiMotes!" <<std::endl;
    if(found)
    {
      std::vector<CWiimote>& wiimotes = wii.Connect();
      std::cout << "Connected to " << wiimotes.size() << " Wiimotes" << std::endl;

      //rumble
      for(auto &wiimote : wiimotes)
      {
        wiimote.SetRumbleMode(CWiimote::ON);
      }
      usleep(200000);
      for(auto &wiimote : wiimotes)
      {
        wiimote.SetRumbleMode(CWiimote::OFF);
      }
      for(auto &wiimote : wiimotes)
      {
        wiimote.SetRumbleMode(CWiimote::OFF);
        wiimote.SetMotionSensingMode(CWiimote::ON);
        wiimote.EnableMotionPlus(CWiimote::ON);
      }
    }
  }
  //start poll loop
  pool_thread = new std::thread(poll_loop_dummy, this);
}

WiiController::~WiiController()
{
  stop = true;
  delete pool_thread;
}

int LED_MAP[4] = {CWiimote::LED_1, CWiimote::LED_2, CWiimote::LED_3, CWiimote::LED_4};


void WiiController::reset_gyro_angles(float roll, float pitch, float yaw)
{
  roll_gyro_angle = roll;
  pitch_gyro_angle = pitch;
  yaw_gyro_angle = yaw;
}
void WiiController::reset_combined_angles(float roll, float pitch, float yaw)
{
  roll_combined_angle = roll;
  pitch_combined_angle = pitch;
  yaw_combined_angle = yaw;
}


void WiiController::HandleEvent(CWiimote &wm)
{


    if(wm.Buttons.isPressed(CButtons::BUTTON_A))
      buttons[BUT_A] = true;
    else
      buttons[BUT_A] = false;

    if(wm.Buttons.isPressed(CButtons::BUTTON_B))
      buttons[BUT_B] = true;
    else
      buttons[BUT_B] = false;

    if(wm.Buttons.isPressed(CButtons::BUTTON_UP))
      buttons[BUT_UP] = true;
    else
      buttons[BUT_UP] = false;


    if(wm.Buttons.isPressed(CButtons::BUTTON_DOWN))
      buttons[BUT_DOWN] = true;
    else
      buttons[BUT_DOWN] = false;


    if(wm.Buttons.isPressed(CButtons::BUTTON_LEFT))
      buttons[BUT_LEFT] = true;
    else
      buttons[BUT_LEFT] = false;

    if(wm.Buttons.isPressed(CButtons::BUTTON_RIGHT))
      buttons[BUT_RIGHT] = true;
    else
      buttons[BUT_RIGHT] = false;

    if(wm.Buttons.isPressed(CButtons::BUTTON_MINUS))
      buttons[BUT_MINUS] = true;
    else
      buttons[BUT_MINUS] = false;

    if(wm.Buttons.isPressed(CButtons::BUTTON_PLUS))
      buttons[BUT_PLUS] = true;
    else
      buttons[BUT_PLUS] = false;

    if(wm.Buttons.isPressed(CButtons::BUTTON_ONE))
      buttons[BUT_ONE] = true;
    else
      buttons[BUT_ONE] = false;

    if(wm.Buttons.isPressed(CButtons::BUTTON_TWO))
      buttons[BUT_TWO] = true;
    else
      buttons[BUT_TWO] = false;

    if(wm.Buttons.isPressed(CButtons::BUTTON_HOME))
      buttons[BUT_HOME] = true;
    else
      buttons[BUT_HOME] = false;

    if(wm.isUsingACC())
    {
        wm.Accelerometer.GetOrientation(pitch_acc, roll_acc, yaw_acc);
    }

    if(wm.isUsingMotionPlus()) {
    	wm.ExpansionDevice.MotionPlus.Gyroscope.GetRates(roll_gyro,pitch_gyro,yaw_gyro);
      if(abs(yaw_gyro) < 0.5) yaw_gyro = 0; //for preventing drift. //hopefully// work.

      roll_gyro_angle += roll_gyro/100.;
      pitch_gyro_angle += pitch_gyro/100.;
      yaw_gyro_angle += yaw_gyro/100.;

      roll_combined_angle = 0.9615 * (roll_combined_angle + roll_gyro/100.) +
        0.0385 * roll_acc;
      pitch_combined_angle = 0.9615 * (pitch_combined_angle + pitch_gyro/100.) +
        0.0385 * pitch_acc;
      yaw_combined_angle += yaw_gyro/100.;

    }
    if(wm.isUsingIR())
    {
        std::vector<CIRDot>::iterator i;
        int x, y;
        int index;

        printf("Num IR Dots: %i\n", wm.IR.GetNumDots());
        printf("IR State: %u\n", wm.IR.GetState());

        std::vector<CIRDot>& dots = wm.IR.GetDots();

        for(index = 0, i = dots.begin(); i != dots.end(); ++index, ++i)
        {
            if((*i).isVisible())
            {
                (*i).GetCoordinate(x, y);
                printf("IR source %i: (%i, %i)\n", index, x, y);

                wm.IR.GetCursorPosition(x, y);
                printf("IR cursor: (%i, %i)\n", x, y);
                printf("IR z distance: %f\n", wm.IR.GetDistance());
            }
        }
    }
}

void WiiController::HandleStatus(CWiimote &wm)
{
    printf("\n");
    printf("--- CONTROLLER STATUS [wiimote id %i] ---\n\n", wm.GetID());

    printf("attachment: %i\n", wm.ExpansionDevice.GetType());
    printf("speaker: %i\n", wm.isUsingSpeaker());
    printf("ir: %i\n", wm.isUsingIR());
    printf("leds: %i %i %i %i\n", wm.isLEDSet(1), wm.isLEDSet(2), wm.isLEDSet(3), wm.isLEDSet(4));
    printf("battery: %f %%\n", wm.GetBatteryLevel());
}

void WiiController::HandleDisconnect(CWiimote &wm)
{
    printf("\n");
    printf("--- DISCONNECTED [wiimote id %i] ---\n", wm.GetID());
    printf("\n");
}

void WiiController::HandleReadData(CWiimote &wm)
{
    printf("\n");
    printf("--- DATA READ [wiimote id %i] ---\n", wm.GetID());
    printf("\n");
}

void WiiController::HandleNunchukInserted(CWiimote &wm)
{
    printf("Nunchuk inserted on controller %i.\n", wm.GetID());
}

bool *WiiController::get_buttons(void)
{
  return buttons;
}
void WiiController::HandleClassicInserted(CWiimote &wm)
{
    printf("Classic controler inserted on controller %i.\n", wm.GetID());
}

void WiiController::HandleGH3Inserted(CWiimote &wm)
{
    printf("GH3 inserted on controller %i.\n", wm.GetID());
}


void WiiController::getacc(float &roll, float &pitch, float &yaw)
{
  roll = roll_acc;
  pitch = pitch_acc;
  yaw = yaw_acc;
}

void WiiController::getgyro(float &roll, float &pitch, float &yaw)
{
  roll = roll_gyro;
  pitch = pitch_gyro;
  yaw = yaw_gyro;
}
void WiiController::getgyroangle(float &roll, float &pitch, float &yaw)
{
  roll = roll_gyro_angle;
  pitch = pitch_gyro_angle;
  yaw = yaw_gyro_angle;
}

void WiiController::getcombinedangle(float &roll, float &pitch, float &yaw)
{
  roll = roll_combined_angle;
  pitch = pitch_combined_angle;
  yaw = yaw_combined_angle;
}


void WiiController::poll_loop()
{
  int wiimote_count = 1;
  int reloadWiimotes = 0;
  int8_t counter = 0;
  int prescaler = 0;
  std::vector<CWiimote>& wiimotes = wii.GetWiimotes();;
  wiimote_count = wiimotes.size();

  while(!stop && wiimote_count)
  {
    if(reloadWiimotes)
    {
        // Regenerate the list of wiimotes
        wiimotes = wii.GetWiimotes();
        reloadWiimotes = 0;
    }

    //Poll the wiimotes to get the status like pitch or roll
    if(wii.Poll())
    {
      CWiimote &wiimote = wiimotes[0];
      //Make LED's go crazy
      if(prescaler++ == 4 )
      {
        prescaler = 0;
        counter+=16;
        wiimote.SetLEDs( counter );
      }

      switch(wiimote.GetEvent())
      {

        case CWiimote::EVENT_EVENT:
          HandleEvent(wiimote);
          break;

        case CWiimote::EVENT_STATUS:
          HandleStatus(wiimote);
          break;

        case CWiimote::EVENT_DISCONNECT:
        case CWiimote::EVENT_UNEXPECTED_DISCONNECT:
          HandleDisconnect(wiimote);
          reloadWiimotes = 1;
          break;

        case CWiimote::EVENT_READ_DATA:
          HandleReadData(wiimote);
          break;

        case CWiimote::EVENT_NUNCHUK_INSERTED:
          HandleNunchukInserted(wiimote);
          reloadWiimotes = 1;
          break;

        case CWiimote::EVENT_CLASSIC_CTRL_INSERTED:
          HandleClassicInserted(wiimote);
          reloadWiimotes = 1;
          break;

        case CWiimote::EVENT_GUITAR_HERO_3_CTRL_INSERTED:
          HandleGH3Inserted(wiimote);
          reloadWiimotes = 1;
          break;

        case CWiimote::EVENT_MOTION_PLUS_INSERTED:
          cout << "Motion Plus inserted." << endl;
          break;

        case CWiimote::EVENT_BALANCE_BOARD_INSERTED:
          cout << "Balance Board connected.\n"  << endl;
          break;

        case CWiimote::EVENT_BALANCE_BOARD_REMOVED:
           cout << "Balance Board disconnected.\n"  << endl;
          break;

        case CWiimote::EVENT_NUNCHUK_REMOVED:
        case CWiimote::EVENT_CLASSIC_CTRL_REMOVED:
        case CWiimote::EVENT_GUITAR_HERO_3_CTRL_REMOVED:
        case CWiimote::EVENT_MOTION_PLUS_REMOVED:
          cout << "An expansion was removed." << endl;
          HandleStatus(wiimote);
          reloadWiimotes = 1;
          break;

          default:
          break;
      }
    }
  }
}

static void poll_loop_dummy(WiiController *obj)
{
  obj->poll_loop();
}
