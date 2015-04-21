#include "fpu.h"
void enable_fpu()
{
  __asm__ volatile
  (
  " LDR.W   r0, =0xE000ED88\n"
  " LDR R1, [r0]\n"
  " ORR   r1, r1, #(0xF << 20)\n"
  " STR   R1, [R0]\n"
  " DSB\n"
  " ISB\n"
  );
}
