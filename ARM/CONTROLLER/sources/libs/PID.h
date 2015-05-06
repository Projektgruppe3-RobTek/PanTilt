#include "average.h"
typedef struct {
  //PID constants
  double last_sample;
  double s_last_sample;
  double last_sum;
  double a;
  double b;
  double c;

  } PID_s;



double PID(PID_s *PID_prop, double input);
PID_s init_PID(double K_p, double K_i, double K_d, double Ts);
