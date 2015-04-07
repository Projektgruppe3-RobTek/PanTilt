#ifndef _PRINT_H_
#define _PRINT_H_

#include <stdarg.h>
#include "../../headers/emp_type.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#define INT_DIGITS 19 //Max digits of 64 bit int.
#define MAX_LINE_LENGHT 4000
//Have to return int as vprintf is reserverd
__attribute__((format(printf, 3, 4))) int vprintf_(void (*destfunc)(char *), size_t max_size, char *string, ...);
INT8U itoa(INT64S number, char * string, INT8U base);
#endif
