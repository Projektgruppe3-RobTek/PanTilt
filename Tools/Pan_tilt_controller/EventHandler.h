#pragma once
#include <SDL2/SDL.h>
#include "structs.h"
#include "Timer.h"
#include "controller_com.h"

#define SPEED 200. //changes per second when holding arrow keys
#define CHANGE (SPEED / FPS)
class EventHandler
{
    private:
        SDL_Window *window;
        SDL_Renderer* renderer;
        coordinate *pan_tilt_coordinate;
        float x = 0;
        float y = 0;
        bool *glob_stop;
    public:
        EventHandler(SDL_Window *window_, SDL_Renderer* renderer_, coordinate *pan_tilt_coordinate_, bool *glob_stop_);
        void handleEvents();
        void stateHandler();
    private:
        void handleKeyUpEvent(SDL_Event &event);
        void handleKeyDownEvent(SDL_Event &event);
        void handleMouseDownEvent(SDL_Event &event);
        void handleWindowEvent(SDL_Event &event);;

};
