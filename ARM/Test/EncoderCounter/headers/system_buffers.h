#pragma once
#include "ringbuffer.h"
INT8U sys_ringbuf_uchar_pop(INT8U id);

void sys_ringbuf_uchar_push(INT8U id, INT8U val);

void sys_ringbuf_uchar_init(void);

INT8U sys_ringbuf_uchar_request(void); //id 0xFF is none allocated


void sys_ringbuf_uchar_release(INT8U id);

bool sys_ringbuf_uchar_full(INT8U id);

RBUF_INDEX_TYPE sys_ringbuf_uchar_size(INT8U id);
RBUF_INDEX_TYPE sys_ringbuf_uchar_max(INT8U id);
