#include "average.h"
void add_value(avg_s *avg_struct, double newvalue)
{
  double total = avg_struct->average * avg_struct->number_of_samples;
  total += newvalue;
  avg_struct->average = total / (avg_struct->number_of_samples + 1);

  if(avg_struct->number_of_samples < MIN_WEIGHT)
    avg_struct->number_of_samples++;
}

double normailze(avg_s *avg_struct, double value, double norm_val)
{
  double normalised = value * (avg_struct->average / norm_val);
  return normalised;
}
