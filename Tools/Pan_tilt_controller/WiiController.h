#pragma once
#include <vector>
#include <wiicpp.h>
#include <thread>

#define   BUT_A  0
#define   BUT_B  1
#define   BUT_ONE  2
#define   BUT_TWO  3
#define   BUT_MINUS  4
#define   BUT_PLUS  5
#define   BUT_HOME  6
#define   BUT_UP  7
#define   BUT_DOWN  8
#define   BUT_LEFT  9
#define   BUT_RIGHT  10


class WiiController
{
  public:
    WiiController();
    ~WiiController();
    void getgyro(float &roll, float &pitch, float &yaw);
    void getgyroangle(float &roll, float &pitch, float &yaw);
    void getcombinedangle(float &roll, float &pitch, float &yaw);
    void reset_gyro_angles(float roll, float pitch, float yaw);
    void reset_combined_angles(float roll, float pitch, float yaw);
    void getacc(float &roll, float &pitch, float &yaw);
    bool *get_buttons(void);
    void poll_loop();
  private:
    bool stop = false;
    CWii wii;
    float roll_acc  = 0;
    float pitch_acc = 0;
    float yaw_acc   = 0;
    float roll_gyro  = 0;
    float pitch_gyro = 0;
    float yaw_gyro   = 0;
    float roll_gyro_angle = 0;
    float pitch_gyro_angle = 0;
    float yaw_gyro_angle = 0;
    float roll_combined_angle = 0;
    float pitch_combined_angle = 0;
    float yaw_combined_angle = 0;
    bool buttons[11];
    void HandleEvent(CWiimote &wm);
    void HandleStatus(CWiimote &wm);
    void HandleDisconnect(CWiimote &wm);
    void HandleReadData(CWiimote &wm);
    void HandleNunchukInserted(CWiimote &wm);
    void HandleClassicInserted(CWiimote &wm);
    void HandleGH3Inserted(CWiimote &wm);
    std::thread *pool_thread;

};
