#include "../headers/pe1.h"
INT32U pulses = 0;

void PE_int(void);

void PE_int(void)
{
	GPIO_PORTE_ICR_R = 1<<1;
	pulses++;
}
