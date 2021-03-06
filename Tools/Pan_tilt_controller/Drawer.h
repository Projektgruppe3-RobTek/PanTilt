#pragma once
#include "structs.h"
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include "EventHandler.h"
#include <string>
#include "WiiController.h"

#define WINDOWSIZE_X 750
#define WINDOWSIZE_Y 600
#define ASIXSCALE 0.8
#define SNAKELENGHT 50

class drawer
{
  private:
    SDL_Window *window;
    SDL_Renderer* renderer;
    Timer timer;
    EventHandler *e_handler;
    coordinate pan_tilt_coordinate;
    controller_communicator *arm_board;
    void draw_axis();
    void draw();
    void draw_position();
    void draw_text();
    bool stop = false;
    std::vector<Motor> motor_pos1;
    std::vector<Motor> motor_pos2;
  public:
    drawer(std::string port, int baudrate, WiiController *wii_controller);
    ~drawer();
    void loop();
};
