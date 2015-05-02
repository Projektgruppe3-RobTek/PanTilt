#include "Drawer.h"
#include <iostream>
#include "WiiController.h"

int main(int argv, char **args)
{
  int baud_rate = 115200;
  std::string port;
  bool wiicontrol = 0;
  for(int i = 1; i < argv; i++)
  {
    std::string arg_str = args[i];
    if (arg_str == "-port")
    {
      if(argv > i + 1)
      {
        port = std::string(args[i + 1]);
      }
    }
    else if (arg_str == "-baud")
    {
      if(argv > i + 1)
      {
        baud_rate =std::stoi(std::string(args[i+1]));
      }
    }
    else if (arg_str == "-wiimote")
    {
      wiicontrol = 1;
    }
  }

  if(port == "")
  {
    std::cout << "Please give a portname: -port <port>" << std::endl;
    abort();
  }
  else
    std::cout << port << std::endl;

  WiiController *wii_controller = nullptr;
  if(wiicontrol)
  {
    wii_controller = new WiiController();
  }
  drawer main_drawer(port, baud_rate, wii_controller);
  main_drawer.loop();
  if(wiicontrol)
  {
    delete wii_controller;
  }
}
