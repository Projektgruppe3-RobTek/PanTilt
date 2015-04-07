#pragma once
#include "../../headers/tm4c123gh6pm.h"

#define INDEX_PORT GPIO_PORTE_DATA_R
#define INDEX_0 (1 << 1)
#define INDEX_1 (1 << 2)

void setup_index_sensors(void);
