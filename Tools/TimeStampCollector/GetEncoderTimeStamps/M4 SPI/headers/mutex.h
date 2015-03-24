#pragma once

#include <stdint.h>

#define LOCKED		1
#define	UNLOCKED	0

typedef uint32_t mutex_t;

extern void lock_mutex(volatile mutex_t* mutex);
extern void unlock_mutex(volatile mutex_t* mutex);
