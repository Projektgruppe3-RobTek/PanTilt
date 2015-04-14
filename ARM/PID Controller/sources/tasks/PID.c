#include "PID.h"
double P_;
double I_;
double D_;

static double P();
static double I();
static double D();

static double P()
{
  return 0;
}

static double I()
{
  return 0;
}
static double D()
{
  return 2;
}

void PID()
{

}

void setup_PID(double P_const, double I_const, double D_const)
{
  P_ = P_const;
  I_ = I_const;
  D_ = D_const;
}
