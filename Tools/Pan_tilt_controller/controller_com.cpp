#include "controller_com.h"
#include <iostream>

static void reciever_dummy(controller_communicator *obj);



controller_communicator::controller_communicator(std::string portname, int buad_rate)
{
  this->port = new boost::asio::serial_port(this->io_service, portname);
  this->port->set_option( boost::asio::serial_port_base::baud_rate( buad_rate ) );
}

controller_communicator::~controller_communicator()
{
  stop_reciever();
  delete port;
}

void controller_communicator::set_pos(bool motor, int16_t position)
{
  uint16_t data = position;
  data &= ~(1 << 15);
  data &= ~(1 << 14);
  data &= ~(1 << 13);
  data |= motor << 14;
  data |= (position < 0) << 13;
  uint8_t first = data >> 8;
  uint8_t last  = data & 0xff;
	boost::asio::write(*(this->port), boost::asio::buffer(&first, 1));
  boost::asio::write(*(this->port), boost::asio::buffer(&last, 1));

}

void controller_communicator::send_reset()
{
  uint16_t data = 1 << 15;
  uint8_t first = data >> 8;
  uint8_t last  = data & 0xff;
	boost::asio::write(*(this->port), boost::asio::buffer(&first, 1));
  boost::asio::write(*(this->port), boost::asio::buffer(&last, 1));
}

char controller_communicator::read_char() //read a char from the serial port
{
  char out_char;
  while(boost::asio::read(*(this->port), boost::asio::buffer(&out_char, 1)) == 0);
  return out_char;
}


std::string controller_communicator::read_line() //read a line from the serial port
{
  std::string out_string = "";
  char out_char;
  while( (out_char = read_char())  != '\n' )
  {
    out_string += out_char;
  }
  return out_string;
}

void controller_communicator::start_reciever()
{
  reciever_thread = new std::thread(reciever_dummy, this);
}

void controller_communicator::stop_reciever()
{
  if(reciever_thread != nullptr && reciever_thread->joinable())
  {
    this->stop = true;
    reciever_thread->detach();
    //memory leak here as we don't delete thread, but whatever, best we can do to exist gracefully atm.
  }
}

void controller_communicator::reciever()
{
  while(!stop)
  {
    std::cout << read_char();
  }
}

static void reciever_dummy(controller_communicator *obj)
{
  obj->reciever();
}
