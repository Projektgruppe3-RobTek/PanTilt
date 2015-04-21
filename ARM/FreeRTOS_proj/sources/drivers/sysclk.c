#include "sysclk.h"

void set_sysclk(INT32U freq)
{
  //Bypass PLL and clock divisors, and clear sysdiv bits
	SYSCTL_RCC_R |= SYSCTL_RCC_BYPASS;
	SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;
	SYSCTL_RCC_R &= ~SYSCTL_RCC_USESYSDIV;
	
	//Use RCC2 registers
	SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;
	
	//Enable main oscilator.
	
	SYSCTL_RCC_R |= SYSCTL_RCC_MOSCDIS;
	
	//Clear PLL Lock.
	SYSCTL_RIS_R = ~SYSCTL_RIS_PLLLRIS;
	
	//Set XTAL val.
	SYSCTL_RCC_R &= ~SYSCTL_RCC_XTAL_M;
	SYSCTL_RCC_R |= XTAL_CLK;
	
	//Set sysclk to use main oscilator
	SYSCTL_RCC_R &= ~SYSCTL_RCC_OSCSRC_M;
	SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;
	SYSCTL_RCC_R |= SYSCTL_RCC_OSCSRC_MAIN;
	SYSCTL_RCC2_R |= SYSCTL_RCC2_OSCSRC2_MO;
	

	SYSCTL_RCC_R &= ~SYSCTL_RCC_PWRDN;
	SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;
	
	//Delay a bit, so the clock can stabilize.
	for(volatile INT32U i = 0; i< 50000; i++);
	
	//Set SYSDIV.
	SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;
	SYSCTL_RCC2_R &= ~SYSCTL_RCC2_SYSDIV2_M;
	
	//calculate sysdiv
	INT32U SYSDIV = PLL_CLOCK / freq;
	SYSCTL_RCC2_R |= SYSDIV << 22;
	
	//Set USESYS
	SYSCTL_RCC_R |= SYSCTL_RCC_USESYSDIV;
	
	// wait for PLL lock
	while( !(SYSCTL_RIS_R & SYSCTL_RIS_PLLLRIS) );
	

	//Clear bypass
	SYSCTL_RCC_R &= ~SYSCTL_RCC_BYPASS;
	SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;
}

INT32U get_sysclk(void) //return current frequency.
{
  return (SYSCTL_RCC2_R & SYSCTL_RCC2_SYSDIV2_M) >> 22;
}
