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
#define SAMPLE_TIME (1000000 / SAMPLE_FREQ) //sample time in µs
#define SAMPLER2_CALIB_PWM 65
#define SAMPLER1_CALIB_PWM 25

typedef struct
{
  double speed;
  double position;
  INT32U time_delta;
} sample_element;


extern xQueueHandle sampler1_queue;
extern xQueueHandle sampler2_queue;
extern xSemaphoreHandle sampler1_queue_sem;
extern xSemaphoreHandle sampler2_queue_sem;



void sampler1_task(void __attribute__((unused)) *pvParameters);
void sampler2_task(void __attribute__((unused)) *pvParameters);
void init_sampler1(void);
void init_sampler2(void);
void calibrate_sampler2(void);
void calibrate_sampler1(void);
void set_pwm1(INT8S pwm);
void set_pwm2(INT8S pwm);
INT32S get_position1(void);
INT32S get_position2(void);
