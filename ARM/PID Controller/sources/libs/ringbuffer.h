#include "../../headers/emp_type.h"
#include <stdbool.h>
#include <stddef.h>
#include "../os/mutex.h"

#pragma once

#define BUFFER_SIZE 200
#define BUFFER_NUM 10

#if BUFFER_SIZE<(1<<8-1)
#define RBUF_INDEX_TYPE INT8U
#elif BUFFER_SIZE<(1<<16-1)
#define RBUF_INDEX_TYPE INT16U
#elif BUFFER_SIZE<(1<<32-1)
#define RBUF_INDEX_TYPE INT32U
#else
#define RBUF_INDEX_TYPE INT64U
#endif
typedef struct
{
	RBUF_INDEX_TYPE max;
	RBUF_INDEX_TYPE head;
	RBUF_INDEX_TYPE tail;
	RBUF_INDEX_TYPE size;
	INT8U buffer[BUFFER_SIZE];
	mutex_t mutex;
} ringbuffer_uchar;

INT8U ringbuffer_uchar_pop(ringbuffer_uchar *buffer);

void ringbuffer_uchar_push(ringbuffer_uchar *buffer, INT8U value);

bool ringbuffer_uchar_full(ringbuffer_uchar *buffer);

void ringbuffer_uchar_init(ringbuffer_uchar *buffer);
