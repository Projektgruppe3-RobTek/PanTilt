#pragma once
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "queue.h"
#include "semphr.h"
#include "../../headers/emp_type.h"
#include "../../headers/GLOBAL_DEFINITIONS.h"
#include <stdbool.h>
#include "task.h"
void uart_control(void __attribute__((unused)) *pvParameters);

#define UART_TASK_FREQ 100
