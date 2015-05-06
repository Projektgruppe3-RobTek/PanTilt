#include "average.h"
typedef struct {
  //PID constants
  double b_0;
  double b_1;
  double b_2;
  double a_1;
  double a_2;
  double last_sample;
  double s_last_sample;
  double last_sum;
  double s_last_sum;


  //Internal variables
  double integral;
  avg_s error_running;
  } PID_s;



double PID(PID_s *PID_prop, double input);
PID_s init_PID(double b_0, double b_1, double b_2, double a_1, double a_2);
