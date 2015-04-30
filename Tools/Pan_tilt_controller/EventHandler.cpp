#include "EventHandler.h"
#include <iostream>
EventHandler::EventHandler(SDL_Window *window_, SDL_Renderer* renderer_, coordinate *pan_tilt_coordinate_, bool *glob_stop_)
{
    window=window_;
    renderer=renderer_;
    pan_tilt_coordinate = pan_tilt_coordinate_;
    glob_stop = glob_stop_;
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