#include "PID.h"
double PID(PID_s *PID_struct, double error)
{
  //Propotional gain
  double proportional_part = error * PID_struct->K_p;

  //Integral gain
  PID_struct->integral += error;
  double integral_part  = PID_struct->integral * PID_struct->K_i;

  //derivative gain.
  double derivative_part   = PID_struct->K_d * (error - PID_struct->last_error);
  PID_struct -> last_error = error;

  return proportional_part + integral_part + derivative_part;

}

PID_s init_PID(double K_p, double K_i, double K_d)
{
  PID_s PID_struct;
  PID_struct.K_p = K_p;
  PID_struct.K_i = K_i;
  PID_struct.K_d = K_d;

  PID_struct.integral   = 0;
  PID_struct.last_error = 0;

  return PID_struct;
}
