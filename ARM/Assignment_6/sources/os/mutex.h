#pragma once

#include <stdint.h>
#include <stdbool.h>

#define LOCKED		1
#define	UNLOCKED	0

typedef uint32_t mutex_t;

void lock_mutex(volatile mutex_t* mutex); //locks the mutex

//tries to lock the mutex, returns true on success, false on fail
bool try_lock_mutex(volatile mutex_t* mutex);

//unlock the mutex
void unlock_mutex(volatile mutex_t* mutex);

//Tests if the mutex is locked.
bool is_mutex_unlocked(mutex_t *mutex);
