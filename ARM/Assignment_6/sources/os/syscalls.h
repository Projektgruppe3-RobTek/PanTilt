#ifndef __SYSCALLS_H_
#define __SYSCALLS_H_

#include <stdint.h>
#include <stddef.h>
void *sbrk(intptr_t increment);
void *_sbrk_r(intptr_t increment);

#endif
