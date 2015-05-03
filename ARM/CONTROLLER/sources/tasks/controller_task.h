#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "../../headers/emp_type.h"

#define GOAL_1 0
#define GOAL_2 270

void controller1_task(void __attribute__((unused)) *pvParameters);
void controller2_task(void __attribute__((unused)) *pvParameters);

void set_goal1(INT16S goal);
void set_goal2(INT16S goal);

extern xTaskHandle controller1_handle;
extern xTaskHandle controller2_handle;
