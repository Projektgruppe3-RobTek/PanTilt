#include "PID.h"
double PID(PID_s *PID_struct, double error)
{
  //Propotional gain
  double sum = PID_struct->b_0 * error;
  sum += PID_struct->b_1 * PID_struct->last_sample;
  sum += PID_struct->b_2 * PID_struct->s_last_sample;
  sum -= PID_struct->a_1 * PID_struct->last_sum;
  sum -= PID_struct->a_2 * PID_struct->s_last_sum;

  PID_struct->s_last_sample = PID_struct->last_sample;
  PID_struct->last_sample = error;

  PID_struct->s_last_sum = PID_struct->last_sum;
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
  return sum;

}

PID_s init_PID(double b_0, double b_1, double b_2, double a_1, double a_2)
{
  PID_s PID_struct;
  PID_struct.b_0 = b_0;
  PID_struct.b_1 = b_1;
  PID_struct.b_2 = b_2;
  PID_struct.a_1 = a_1;
  PID_struct.a_2 = a_2;

  PID_struct.last_sample = 0;
  PID_struct.s_last_sample = 0;
  PID_struct.last_sum = 0;
  PID_struct.s_last_sum = 0;


  PID_struct.integral   = 0;
  PID_struct.error_running.average = 0;
  PID_struct.error_running.number_of_samples = 0;
  PID_struct.error_running.max_samples = 10;

  return PID_struct;
}
