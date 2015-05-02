
typedef struct {
  //PID constants
  double K_p;
  double K_i;
  double K_d;

  //Internal variables
  double integral;
  double last_error;
  } PID_s;



double PID(PID_s *PID_prop, double input);
PID_s init_PID(double K_p, double K_i, double K_d);
