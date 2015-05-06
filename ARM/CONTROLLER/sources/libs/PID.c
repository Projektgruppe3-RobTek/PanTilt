#include "PID.h"
#include <math.h>
double PID(PID_s *PID_struct, double error)
{
  //Propotional gain
  double sum =  PID_struct->last_sum;
  sum += PID_struct->a * error;
  sum += PID_struct->b * PID_struct->last_sample;
  sum += PID_struct->c * PID_struct->s_last_sample;

  double return_val = sum;

  PID_struct->s_last_sample = PID_struct->last_sample;
  PID_struct->last_sample = error;
  PID_struct->last_sum = sum;





  //double proportional_part = error * PID_struct->K_p;

  //Integral gain
  //PID_struct->integral += error;
  //double integral_part  = PID_struct->integral * PID_struct->K_i;

  //derivative gain.
  /*double old_avg = PID_struct->error_running.average;
  add_value(&(PID_struct->error_running), error);

  double derivative_part   = PID_struct->K_d * (PID_struct->error_running.average - old_avg);*/
  //return proportional_part + integral_part + derivative_part;
  return return_val;

}

PID_s init_PID(double K_p, double K_i, double K_d, double Ts)
{
  PID_s PID_struct;
  PID_struct.last_sample = 0;
  PID_struct.s_last_sample = 0;
  PID_struct.last_sum = 0;
  double sampling_time = 1./Ts;
  PID_struct.a = K_p + K_i * (sampling_time / 2) + (K_d / sampling_time);
  PID_struct.b = -K_p + K_i * (sampling_time / 2) - ((2 * K_d) / sampling_time);
  PID_struct.c = K_d / sampling_time;



  return PID_struct;
}
