#pragma once
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "queue.h"
#include "semphr.h"
#include "../../headers/emp_type.h"
#include "../../headers/GLOBAL_DEFINITIONS.h"
#include <stdbool.h>
#define TIME_RES 80//resulution of time in ns
#define SAMPLE_FREQ 1000 // HZ
#define SAMPLER2_CALIB_PWM 65
#define SAMPLER1_CALIB_PWM 25

typedef enum
{
  speed,
  position
} element_type ;

typedef struct
{
  element_type type;
  double value;
} sample_element;


extern xQueueHandle sampler1_queue;
extern xQueueHandle sampler2_queue;
extern xSemaphoreHandle sampler1_queue_sem;
extern xSemaphoreHandle sampler2_queue_sem;
extern INT8S pwm_motor1;
extern INT8S pwm_motor2;
extern bool reset;



void sampler1_task(void __attribute__((unused)) *pvParameters);
void sampler2_task(void __attribute__((unused)) *pvParameters);
void init_sampler1(void);
void init_sampler2(void);
void calibrate_sampler2(void);
void calibrate_sampler1(void);
