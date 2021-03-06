//code for keeping a live, updating, average
//new values a waited less and less when more are added
#include "../../headers/emp_type.h"

#define MIN_WEIGHT 10 //a sample will not be weighted less than this
#pragma once

typedef struct
{
  double average;
  INT32U number_of_samples;
  INT32U max_samples;
} avg_s;

void add_value(avg_s *avg_struct, double newvalue);
double normalize(avg_s *avg_struct, double value, double norm_val);
