
typedef struct {
  //PID constants
  double K_p;
  double K_i;
  double K_d;

  //Internal variables
  } PID_s;



double PID(PID_s *PID_prop, double input);
