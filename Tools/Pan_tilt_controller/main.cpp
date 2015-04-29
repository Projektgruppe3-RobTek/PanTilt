#include "Drawer.h"
#include <iostream>


int main(int argv, char **args)
{
  int baud_rate = 115200;
  std::string port;
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
  }

  if(port == "")
  {
    std::cout << "Please give a portname: -port <port>" << std::endl;
    abort();
  }
  else
    std::cout << port << std::endl;

  drawer main_drawer(port, baud_rate);
  main_drawer.loop();
}
