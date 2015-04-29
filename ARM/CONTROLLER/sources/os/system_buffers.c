#include "system_buffers.h"
ringbuffer_uchar system_buffers[BUFFER_NUM];
bool allocated_buffers[BUFFER_NUM];


INT8U sys_ringbuf_uchar_pop(INT8U id)
{
	return ringbuffer_uchar_pop(&system_buffers[id]);
}

void sys_ringbuf_uchar_push(INT8U id, INT8U val)
{
	ringbuffer_uchar_push(&system_buffers[id], val);
}

void sys_ringbuf_uchar_init()
{
	for(INT8U i = 0; i < BUFFER_NUM; i++)
	{
		ringbuffer_uchar_init(&system_buffers[i]);
		allocated_buffers[i] = false;
	}
}

INT8U sys_ringbuf_uchar_request() //id 0xFF is none allocated_buffers
{
	INT8U returnid = 0xFF;
	for(INT8U i = 0; i < BUFFER_NUM && returnid == 0xFF; i++)
	{
		if(!allocated_buffers[i])
		{
			allocated_buffers[i] = true;
			returnid = i;
		}
	}
	return returnid;
}

void sys_ringbuf_uchar_release(INT8U id)
{
	allocated_buffers[id] = false;
	ringbuffer_uchar_init(&system_buffers[id]);
}

bool sys_ringbuf_uchar_full(INT8U id)
{
	return 	ringbuffer_uchar_full(&system_buffers[id]);
}


RBUF_INDEX_TYPE sys_ringbuf_uchar_size(INT8U id)
{
	return system_buffers[id].size;
}

RBUF_INDEX_TYPE sys_ringbuf_uchar_max(INT8U id)
{
	return system_buffers[id].max;
}
