#include "PID.h"
double PID(PID_s *PID_prop, double error)
{
  //Propotional gain
  double proportional = error * PID_prop->K_p;

  return proportional;
}
