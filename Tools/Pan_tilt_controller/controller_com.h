#pragma once
#include <boost/asio.hpp>
#include <string>
#include <cstdint>
#include <thread>

struct Motor{
	bool id;
	int16_t pos;
};

class controller_communicator
{
  private:
    boost::asio::serial_port *port;
    boost::asio::io_service io_service;
    bool stop = false;
    std::thread *reciever_thread;

  public:
    bool read_char_noblock(char *out_char);
    controller_communicator(std::string portname, int buad_rate);
    ~controller_communicator();
    void set_pos(bool motor, int16_t position);
    Motor get_pos();
    void send_reset();
    char read_char(); //read a char from the serial port
    std::string read_line(); //read a line from the serial port
    void reciever();
    void stop_reciever();
    void start_reciever();
};
