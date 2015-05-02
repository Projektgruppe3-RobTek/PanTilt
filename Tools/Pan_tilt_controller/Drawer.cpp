#include "Drawer.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include "Primitives.h"
#include "TextDrawer.h"
drawer::drawer(std::string port, int baudrate, WiiController *wii_controller)
:timer(Timer(FPS))
{
  //Init SDL
  pan_tilt_coordinate = {0, 0};

  if(SDL_Init(SDL_INIT_VIDEO) != 0)
  {
      std::cout << "SDL_INIT Error: " << SDL_GetError() << std::endl;
      exit(0);
  }
  // Initialize SDL_ttf library
  if (TTF_Init() != 0)
  {
      std::cerr << "TTF_Init() Failed: " << TTF_GetError() << std::endl;
      SDL_Quit();
      TTF_Quit();
      exit(1);
  }

  //Create a window
  window = SDL_CreateWindow("PanTile Controller", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                      WINDOWSIZE_X, WINDOWSIZE_Y,
                      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if(window == nullptr)
  {
    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    TTF_Quit();
    exit(0);
  }
  //Create Renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(renderer == nullptr)
  {
    SDL_DestroyWindow(window);
    std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    TTF_Quit();
  }
  e_handler = new EventHandler(window, renderer, &pan_tilt_coordinate, &stop, wii_controller);
  arm_board = new controller_communicator(port.c_str(), baudrate);
  //arm_board->start_reciever();
  //load textures for the pieces
}

drawer::~drawer()
{
  delete e_handler;
  delete arm_board;
  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void drawer::loop()
{
  while(!stop)
  {
    e_handler->stateHandler();
    e_handler->handleEvents();
    draw();
    //arm_board->set_pos(0, pan_tilt_coordinate.x);
    //arm_board->set_pos(1, pan_tilt_coordinate.y);
    timer.tick();
  }
}

void drawer::draw()
{
  SDL_SetRenderDrawColor(renderer, 0,0,0,0);
  SDL_RenderClear(renderer);
  draw_axis();
  draw_text();
  SDL_RenderPresent(renderer);
}

void drawer::draw_text()
{
  int w, h;
  SDL_GetWindowSize(window,&w,&h);
  TextDrawer TDrawer("FreeSans.ttf", 15);

  //axis numbers
  for(int i = 0; i < 10; i++)
  {
    //x
    TDrawer.DrawText(renderer, std::to_string((i * w / 10 - w/2) * ASIXSCALE ).c_str(), i * w / 10 - 3, - (pan_tilt_coordinate.y / ASIXSCALE) + h/2 + w/60, 0, 255, 0, 255);

    //y
    TDrawer.DrawText(renderer, std::to_string((-(i * h / 10 - h/2)) * ASIXSCALE ).c_str(), (pan_tilt_coordinate.x / ASIXSCALE) + w/2 + w/40, i * h / 10 -10, 0, 255, 0, 255);
  }

  //info boxes
  std::string pos_string = "Current position: (";
  pos_string += std::to_string(pan_tilt_coordinate.x);
  pos_string += ",";
  pos_string += std::to_string(pan_tilt_coordinate.y);
  pos_string += ")";

  TDrawer.DrawText(renderer, pos_string.c_str(), 0, 0, 0, 255, 0, 255);


}

void drawer::draw_axis()
{
  int w, h;
  SDL_GetWindowSize(window,&w,&h);

  //draw vertical line
  SDL_SetRenderDrawColor(renderer, 0,0,255,0);


  SDL_RenderDrawThickLine(renderer, 0, -(pan_tilt_coordinate.y / ASIXSCALE) + h/2, w, -(pan_tilt_coordinate.y / ASIXSCALE) + h/2, w/100); //draw x-axis
  //draw y-axis
  SDL_RenderDrawThickLine(renderer, (pan_tilt_coordinate.x / ASIXSCALE) + w/2, 0,
    (pan_tilt_coordinate.x / ASIXSCALE) + w/2, h, w/100); //draw x-axis

  SDL_SetRenderDrawColor(renderer, 255,0,0,0);
  drawFilledCircle(renderer, (pan_tilt_coordinate.x / ASIXSCALE) + w/2, -(pan_tilt_coordinate.y / ASIXSCALE) + h/2, w/75);


}
