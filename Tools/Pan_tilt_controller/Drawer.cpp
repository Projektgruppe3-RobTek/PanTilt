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
  pan_tilt_coordinate = {0, 0, 0};

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
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
  double t = 0;
  double last_pos = 0;
  while(!stop)
  {
    e_handler->stateHandler();
    e_handler->handleEvents();
    arm_board->set_pos(0, -pan_tilt_coordinate.x);
    arm_board->set_pos(1, -pan_tilt_coordinate.y);
    if(pan_tilt_coordinate.reset)
    {
      arm_board->send_reset();
      pan_tilt_coordinate.reset = false;
    }
    Motor motor1 = arm_board->get_pos();
    Motor motor2 = arm_board->get_pos();
    //std::cout << std::endl;


    std::cout << -pan_tilt_coordinate.y << "\t";
    std::cout << -pan_tilt_coordinate.x << "\t";

    std::cout << motor2.pos << "\t";
    std::cout << motor1.pos << "\t";
    std::cout << std::endl;
    if(motor1.id == 0)
    {
      motor_pos1.push_back(motor1);
    }
    else
    {
      motor_pos1.push_back(motor1);
    }

    if(motor2.id == 0)
    {
      motor_pos2.push_back(motor2);
    }
    else
    {
      motor_pos2.push_back(motor2);
    }

    while(motor_pos1.size() > SNAKELENGHT)
    {
      motor_pos1.erase(motor_pos1.begin());
    }

    while(motor_pos2.size() > SNAKELENGHT)
    {
      motor_pos2.erase(motor_pos2.begin());
    }
    draw();
    timer.tick();
  }
}

void drawer::draw()
{
  SDL_SetRenderDrawColor(renderer, 0,0,0,0);
  SDL_RenderClear(renderer);
  draw_axis();
  draw_position();
  draw_text();
  SDL_RenderPresent(renderer);
}

void drawer::draw_position()
{
  int w, h;
  SDL_GetWindowSize(window,&w,&h);

  for(int i = 0; i < motor_pos1.size() - 1; i++)
  {
    SDL_SetRenderDrawColor(renderer, (50./motor_pos1.size()) * i + 100, (50./motor_pos1.size()) * i + 50, (50./motor_pos1.size()) * i + 50,0);

    drawFilledCircle(renderer, (-motor_pos1[i].pos / ASIXSCALE) + w/2, -(-motor_pos2[i].pos / ASIXSCALE) + h/2, (w/100. / motor_pos1.size() * i) );
  }
  SDL_SetRenderDrawColor(renderer, 255, 255,125,0);
  drawFilledCircle(renderer, (-motor_pos1.back().pos / ASIXSCALE) + w/2, -(-motor_pos2.back().pos / ASIXSCALE) + h/2, w/100 );
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
  std::string pos_string = "Set position: (";
  pos_string += std::to_string(pan_tilt_coordinate.x);
  pos_string += ",";
  pos_string += std::to_string(pan_tilt_coordinate.y);
  pos_string += ")";

  std::string measured_pos_string = "Current position: (";
  measured_pos_string += std::to_string(motor_pos1.back().pos);
  measured_pos_string += ",";
  measured_pos_string += std::to_string(motor_pos2.back().pos);
  measured_pos_string += ")";

  TDrawer.DrawText(renderer, pos_string.c_str(), 0, 0, 0, 255, 0, 255);
  TDrawer.DrawText(renderer, measured_pos_string.c_str(), 0, 40, 0, 255, 0, 255);
  TDrawer.DrawText(renderer, std::string(std::to_string(timer.getFPS())).c_str(),0, 80, 0, 255, 0, 255);


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
