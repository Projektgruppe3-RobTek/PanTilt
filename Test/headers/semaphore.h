#pragma once

#include <stdint.h>

typedef uint32_t sem_t;

extern void sem_dec(volatile mutex_t* mutex);
extern void sem_inc(volatile mutex_t* mutex);
