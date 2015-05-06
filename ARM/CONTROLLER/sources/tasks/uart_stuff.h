//Protocol of communication
//|----|----|------------|
//|  R |  M |   POS      |
//|  S |  O |   Signed   |
//|  T |  T |            |
//|----|----|------------|
//RST=Reset.
//Motor = motor to set.
//POS = position, signed.
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
void uart_response(void __attribute__((unused)) *pvParameters);
#define UART_CONTROL_FREQ 300
#define UART_CONTROL_WAIT_TIME (1000000 / UART_CONTROL_FREQ)
#define UART_RESPONSE_FREQ 50
#define UART_RESPONSE_WAIT_TIME (1000000 / UART_RESPONSE_FREQ)
