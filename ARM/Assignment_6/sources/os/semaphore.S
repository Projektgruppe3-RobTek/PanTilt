@Code for semaphores's
@Used http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dht0008a/ch01s03s02.html
@As reference
#include <stdbool.h>

.syntax unified

@ sem_dec
@ Declare for use from C as extern void sem_dec(void * semaphore)@
    .global sem_dec
sem_dec:
1:  LDREX   r1, [r0]
    CMP		  r1, #0        @ Test if semaphore holds the value 0
    BEQ     2f           	@ If it does, block before retrying
    SUB     r1, #1        @ If not, decrement temporary copy
    STREX   r2, r1, [r0]  @ Attempt Store-Exclusive
    CMP     r2, #0        @ Check if Store-Exclusive succeeded
    BNE     1b           	@ If Store-Exclusive failed, retry from start
    DMB                   @ Required before accessing protected resource
    BX      lr

2:   @ Take appropriate action while waiting for semaphore to be incremented
    @WAIT_FOR_UPDATE       @ Wait for signal to retry
    B       1b


@ sem_inc
@ Declare for use from C as extern void sem_inc(void * semaphore)@
    .global sem_inc
sem_inc:
1:   LDREX  r1, [r0]
    ADD     r1, #1        @ Increment temporary copy
    STREX   r2, r1, [r0]  @ Attempt Store-Exclusive
    CMP     r2, #0        @ Check if Store-Exclusive succeeded
    BNE     1b           	@ Store failed - retry immediately
    CMP     r0, #1      	@ Store successful - test if incremented from zero
    DMB                   @ Required before releasing protected resource
    BGE     2f         		@ If initial value was 0, signal update
    BX      lr

2:   @ Signal waiting processors or processes
    @SIGNAL_UPDATE
    BX      lr
    
    
@ try_sem_dec
@ Declare for use from C as extern bool sem_dec(void * semaphore)@
    .global try_sem_dec
try_sem_dec:
1:  LDREX   r1, [r0]
    CMP		  r1, #0        @ Test if semaphore holds the value 0
    BEQ     2f           	@ If it does, return false
    SUB     r1, #1        @ If not, decrement temporary copy
    STREX   r2, r1, [r0]  @ Attempt Store-Exclusive
    CMP     r2, #0        @ Check if Store-Exclusive succeeded
    BNE     2f           	@ If Store-Exclusive failed, return false
    DMB                   @ Required before accessing protected resource
    LDR			r0, = true		@ Return true								
    BX      lr

2:
		@ Return false
		LDR 		r0, = false
		BX			lr
