#include "EventHandler.h"
#include <iostream>
EventHandler::EventHandler(SDL_Window *window_, SDL_Renderer* renderer_, coordinate *pan_tilt_coordinate_, bool *glob_stop_, WiiController *wii_controller_)
{
    window=window_;
    renderer=renderer_;
    pan_tilt_coordinate = pan_tilt_coordinate_;
    glob_stop = glob_stop_;
    wii_controller = wii_controller_;
}

void EventHandler::stateHandler()
{
    int w,h;
    SDL_GetWindowSize(window,&w,&h);
    //Keyboard
    int lenght;
    const Uint8* keyState=SDL_GetKeyboardState(&lenght);
    if(keyState[SDL_SCANCODE_LEFT])
    {
        x-=CHANGE;
        pan_tilt_coordinate->x = (int)x;
    }
    if(keyState[SDL_SCANCODE_RIGHT])
    {
        x+=CHANGE;
        pan_tilt_coordinate->x = (int)x;
    }
    if(keyState[SDL_SCANCODE_UP])
    {
        y+=CHANGE;
        pan_tilt_coordinate->y = (int)y;
    }
    if(keyState[SDL_SCANCODE_DOWN])
    {
        y-=CHANGE;
        pan_tilt_coordinate->y = (int)y;
    }

    //Mouse
    int xpos,ypos;
    if(SDL_GetMouseState(&xpos,&ypos) & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
      pan_tilt_coordinate->x = xpos - w/2;
      x = pan_tilt_coordinate->x;
      pan_tilt_coordinate->y = -(ypos - h/2);
      y= pan_tilt_coordinate->y;
    }
    //wiimote
    if(wii_controller)
    {
      bool has_reset = 0;
      bool *buttons = wii_controller->get_buttons();
      if(buttons[BUT_B])
      {
        wii_controller->reset_combined_angles(0, 0, 0);
      }
      if(buttons[BUT_A])
      {
        float roll, pitch, yaw;
        wii_controller->getcombinedangle(roll, pitch, yaw);
        pan_tilt_coordinate->y = -pitch * ANGLE_TO_PANTILT;
        pan_tilt_coordinate->x = -yaw * ANGLE_TO_PANTILT;
      }
    }
}

void EventHandler::handleEvents()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                *glob_stop = true;
                break;
            case SDL_KEYDOWN:
                handleKeyDownEvent(event);
                break;
            case SDL_KEYUP:
                handleKeyUpEvent(event);
                break;
            case SDL_WINDOWEVENT:
                handleWindowEvent(event);
                break;
            case SDL_MOUSEBUTTONDOWN:
                handleMouseDownEvent(event);
                break;
        }
    }
}

void EventHandler::handleKeyDownEvent(SDL_Event &event)
{

}

void EventHandler::handleKeyUpEvent(SDL_Event &event)
{

}
void EventHandler::handleMouseDownEvent(SDL_Event &event)
{

}
void EventHandler::handleWindowEvent(SDL_Event &event)
{

}
