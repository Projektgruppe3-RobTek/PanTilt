#pragma once

#include "SSI0.h"
#include "SSI2.h"
#include "SSI3.h"


//Define the clock divisors. The formula is BR=SysClk/(CPSDVSR * (1 + SCR))
#define CPSDVSR 2 //must be an even number 2-254
#define SCR 0 //Must be a number 0-255
