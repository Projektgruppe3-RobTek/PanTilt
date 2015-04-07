#pragma once

#include "../../headers/emp_type.h"
#include <stdbool.h>
typedef struct {
	INT8U sec;
	INT8U min;
	INT8U hour;
} time;

#define SECONDS_PER_MINUTE 	60
#define MINUTES_PER_HOUR		60
#define HOURS_PER_DAY				24

void time_count_hour(time *time_s, INT32S count);
void time_count_min(time *time_s, INT32S count);
void time_count_sec(time *time_s, INT32S count);
