#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "../../headers/emp_type.h"

#define GOAL_1 0
#define GOAL_2 270

void controller1_task(void __attribute__((unused)) *pvParameters);
void controller2_task(void __attribute__((unused)) *pvParameters);

extern xTaskHandle controller1_handle;
extern xTaskHandle controller2_handle;

extern INT16S goal1;
extern INT16S goal2;
