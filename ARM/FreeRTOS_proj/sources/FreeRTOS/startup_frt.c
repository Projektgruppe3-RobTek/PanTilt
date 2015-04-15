/**
 ******************************************************************************
 * @file      startup_Cortex_M4.c
 * @author    Coocox
 * @version   V1.0
 * @date      03/16/2013
 * @brief     Cortex M4 Devices Startup code.
 *            This module performs:
 *                - Set the initial SP
 *                - Set the vector table entries with the exceptions ISR address
 *                - Initialize data and bss
 *                - Call the application's entry point.
 *            After Reset the Cortex-M4 processor is in Thread mode,
 *            priority is Privileged, and the Stack is set to Main.
 *******************************************************************************
 */



/*----------Stack Configuration-----------------------------------------------*/

#include <stdint.h>
#define STACK_SIZE       0x00000100      /*!< Stack size (in Words)           */
__attribute__ ((section(".co_stack")))
unsigned long pulStack[STACK_SIZE];


/*----------Macro definition--------------------------------------------------*/
#define WEAK __attribute__ ((weak))


/*----------Declaration of the default fault handlers-------------------------*/
/* System exception vector handler */
__attribute__ ((used))
void WEAK  ResetISR(void);
void WEAK  NMI_Handler(void);
void WEAK  HardFault_Handler(void);
void WEAK  MemManage_Handler(void);
void WEAK  BusFault_Handler(void);
void WEAK  UsageFault_Handler(void);
void WEAK  SVC_Handler(void);
void WEAK  DebugMon_Handler(void);
void WEAK  PendSV_Handler(void);
void WEAK  SysTick_Handler(void);
static void IntDefaultHandler(void);

/* -----------FreeRTOS--------------------- */
void vPortSVCHandler( void ) __attribute__ (( naked ));
void xPortPendSVHandler( void ) __attribute__ (( naked ));
void xPortSysTickHandler( void );
extern void ssi0_isr(void);
extern void ssi3_isr(void);
extern void uart0_isr(void);

/*----------Symbols defined in linker script----------------------------------*/
extern unsigned long __etext;    /*!< Start address for the initialization
                                      values of the .data section.            */
extern unsigned long __data_start__;     /*!< Start address for the .data section     */
extern unsigned long __data_end__;     /*!< End address for the .data section       */
extern unsigned long __bss_start__;      /*!< Start address for the .bss section      */
extern unsigned long __bss_end__;      /*!< End address for the .bss section        */
extern void _eram;               /*!< End address for ram                     */


/*----------Function prototypes-----------------------------------------------*/
extern int main(void);           /*!< The entry point for the application.    */
void Default_ResetISR(void);   /*!< Default reset handler                */
static void Default_Handler(void);  /*!< Default exception handler            */


/**
  *@brief The minimal vector table for a Cortex M4.  Note that the proper constructs
  *       must be placed on this to ensure that it ends up at physical address
  *       0x00000000.
  */
__attribute__ ((used,section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
  /*----------Core Exceptions------------------------------------------------ */
  (void (*)(void))  ((uint32_t)&pulStack[STACK_SIZE-1]),     /*!< The initial stack pointer         */
  ResetISR,             /*!< Reset Handler                               */
  NMI_Handler,               /*!< NMI Handler                                 */
  HardFault_Handler,         /*!< Hard Fault Handler                          */
  MemManage_Handler,         /*!< MPU Fault Handler                           */
  BusFault_Handler,          /*!< Bus Fault Handler                           */
  UsageFault_Handler,        /*!< Usage Fault Handler                         */
  0,0,0,0,                   /*!< Reserved                                    */
  vPortSVCHandler,               /*!< SVCall Handler                              */
  DebugMon_Handler,          /*!< Debug Monitor Handler                       */
  0,                         /*!< Reserved                                    */
  xPortPendSVHandler,            /*!< PendSV Handler                              */
  xPortSysTickHandler,           /*!< SysTick Handler                             */
  IntDefaultHandler,                      // GPIO Port A
  IntDefaultHandler,                      // GPIO Port B
  IntDefaultHandler,                      // GPIO Port C
  IntDefaultHandler,                      // GPIO Port D
  IntDefaultHandler,                      // GPIO Port E
  uart0_isr,				                      // UART0 Rx and Tx
  IntDefaultHandler,                      // UART1 Rx and Tx
  ssi0_isr,                               // SSI0 Rx and Tx
  IntDefaultHandler,                      // I2C0 Master and Slave
  IntDefaultHandler,                      // PWM Fault
  IntDefaultHandler,                      // PWM Generator 0
  IntDefaultHandler,                      // PWM Generator 1
  IntDefaultHandler,                      // PWM Generator 2
  IntDefaultHandler,                      // Quadrature Encoder 0
  IntDefaultHandler,                      // ADC Sequence 0
  IntDefaultHandler,                      // ADC Sequence 1
  IntDefaultHandler,                      // ADC Sequence 2
  IntDefaultHandler,                      // ADC Sequence 3
  IntDefaultHandler,                      // Watchdog timer
  IntDefaultHandler,                      // Timer 0 subtimer A
  IntDefaultHandler,                      // Timer 0 subtimer B
  IntDefaultHandler,                      // Timer 1 subtimer A
  IntDefaultHandler,                      // Timer 1 subtimer B
  IntDefaultHandler,                      // Timer 2 subtimer A
  IntDefaultHandler,                      // Timer 2 subtimer B
  IntDefaultHandler,                      // Analog Comparator 0
  IntDefaultHandler,                      // Analog Comparator 1
  IntDefaultHandler,                      // Analog Comparator 2
  IntDefaultHandler,                      // System Control (PLL, OSC, BO)
  IntDefaultHandler,                      // FLASH Control
  IntDefaultHandler,                      			// GPIO Port F
  IntDefaultHandler,                      // GPIO Port G
  IntDefaultHandler,                      // GPIO Port H
  IntDefaultHandler,                      // UART2 Rx and Tx
  IntDefaultHandler,                      // SSI1 Rx and Tx
  IntDefaultHandler,                      // Timer 3 subtimer A
  IntDefaultHandler,                      // Timer 3 subtimer B
  IntDefaultHandler,                      // I2C1 Master and Slave
  IntDefaultHandler,                      // Quadrature Encoder 1
  IntDefaultHandler,                      // CAN0
  IntDefaultHandler,                      // CAN1
  0,                                      // Reserved
  0,                                      // Reserved
  IntDefaultHandler,                      // Hibernate
  IntDefaultHandler,                      // USB0
  IntDefaultHandler,                      // PWM Generator 3
  IntDefaultHandler,                      // uDMA Software Transfer
  IntDefaultHandler,                      // uDMA Error
  IntDefaultHandler,                      // ADC1 Sequence 0
  IntDefaultHandler,                      // ADC1 Sequence 1
  IntDefaultHandler,                      // ADC1 Sequence 2
  IntDefaultHandler,                      // ADC1 Sequence 3
  0,                                      // Reserved
  0,                                      // Reserved
  IntDefaultHandler,                      // GPIO Port J
  IntDefaultHandler,                      // GPIO Port K
  IntDefaultHandler,                      // GPIO Port L
  IntDefaultHandler,                      // SSI2 Rx and Tx
  ssi3_isr,                               // SSI3 Rx and Tx
  IntDefaultHandler,                      // UART3 Rx and Tx
  IntDefaultHandler,                      // UART4 Rx and Tx
  IntDefaultHandler,                      // UART5 Rx and Tx
  IntDefaultHandler,                      // UART6 Rx and Tx
  IntDefaultHandler,                      // UART7 Rx and Tx
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  IntDefaultHandler,                      // I2C2 Master and Slave
  IntDefaultHandler,                      // I2C3 Master and Slave
  IntDefaultHandler,                      // Timer 4 subtimer A
  IntDefaultHandler,                      // Timer 4 subtimer B
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  0,                                      // Reserved
  IntDefaultHandler,                      // Timer 5 subtimer A
  IntDefaultHandler,                      // Timer 5 subtimer B
  IntDefaultHandler,                      // Wide Timer 0 subtimer A
  IntDefaultHandler,                      // Wide Timer 0 subtimer B
  IntDefaultHandler,                      // Wide Timer 1 subtimer A
  IntDefaultHandler,                      // Wide Timer 1 subtimer B
  IntDefaultHandler,                      // Wide Timer 2 subtimer A
  IntDefaultHandler,                      // Wide Timer 2 subtimer B
  IntDefaultHandler,                      // Wide Timer 3 subtimer A
  IntDefaultHandler,                      // Wide Timer 3 subtimer B
  IntDefaultHandler,                      // Wide Timer 4 subtimer A
  IntDefaultHandler,                      // Wide Timer 4 subtimer B
  IntDefaultHandler,                      // Wide Timer 5 subtimer A
  IntDefaultHandler,                      // Wide Timer 5 subtimer B
  IntDefaultHandler,                      // FPU
  0,                                      // Reserved
  0,                                      // Reserved
  IntDefaultHandler,                      // I2C4 Master and Slave
  IntDefaultHandler,                      // I2C5 Master and Slave
  IntDefaultHandler,                      // GPIO Port M
  IntDefaultHandler,                      // GPIO Port N
  IntDefaultHandler,                      // Quadrature Encoder 2
  0,                                      // Reserved
  0,                                      // Reserved
  IntDefaultHandler,                      // GPIO Port P (Summary or P0)
  IntDefaultHandler,                      // GPIO Port P1
  IntDefaultHandler,                      // GPIO Port P2
  IntDefaultHandler,                      // GPIO Port P3
  IntDefaultHandler,                      // GPIO Port P4
  IntDefaultHandler,                      // GPIO Port P5
  IntDefaultHandler,                      // GPIO Port P6
  IntDefaultHandler,                      // GPIO Port P7
  IntDefaultHandler,                      // GPIO Port Q (Summary or Q0)
  IntDefaultHandler,                      // GPIO Port Q1
  IntDefaultHandler,                      // GPIO Port Q2
  IntDefaultHandler,                      // GPIO Port Q3
  IntDefaultHandler,                      // GPIO Port Q4
  IntDefaultHandler,                      // GPIO Port Q5
  IntDefaultHandler,                      // GPIO Port Q6
  IntDefaultHandler,                      // GPIO Port Q7
  IntDefaultHandler,                      // GPIO Port R
  IntDefaultHandler,                      // GPIO Port S
  IntDefaultHandler,                      // PWM 1 Generator 0
  IntDefaultHandler,                      // PWM 1 Generator 1
  IntDefaultHandler,                      // PWM 1 Generator 2
  IntDefaultHandler,                      // PWM 1 Generator 3
  IntDefaultHandler                       // PWM 1 Fault
};



/**
  * @brief  This is the code that gets called when the processor first
  *         starts execution following a reset event. Only the absolutely
  *         necessary set is performed, after which the application
  *         supplied main() routine is called.
  * @param  None
  * @retval None
  */
void Default_ResetISR(void)
{
  /* Initialize data and bss */
  unsigned long *pulSrc, *pulDest;

  /* Copy the data segment initializers from flash to SRAM */
  pulSrc = &__etext;

  for(pulDest = &__data_start__; pulDest < &__data_end__; )
  {
    *(pulDest++) = *(pulSrc++);
  }

  /* Zero fill the bss segment.  This is done with inline assembly since this
     will clear the value of pulDest if it is not kept in a register. */
  __asm("  ldr     r0, =__bss_start__\n"
        "  ldr     r1, =__bss_end__\n"
        "  mov     r2, #0\n"
        "  .thumb_func\n"
        "zero_loop:\n"
        "    cmp     r0, r1\n"
        "    it      lt\n"
        "    strlt   r2, [r0], #4\n"
        "    blt     zero_loop");

  /* Call the application's entry point.*/
  main();
}


/**
  *@brief Provide weak aliases for each Exception handler to the Default_Handler.
  *       As they are weak aliases, any function with the same name will override
  *       this definition.
  */
#pragma weak ResetISR = Default_ResetISR
#pragma weak NMI_Handler = Default_Handler
#pragma weak HardFault_Handler = Default_Handler
#pragma weak MemManage_Handler = Default_Handler
#pragma weak BusFault_Handler = Default_Handler
#pragma weak UsageFault_Handler = Default_Handler
#pragma weak SVC_Handler = Default_Handler
#pragma weak DebugMon_Handler = Default_Handler
#pragma weak PendSV_Handler = Default_Handler
#pragma weak SysTick_Handler = Default_Handler


/**
  * @brief  This is the code that gets called when the processor receives an
  *         unexpected interrupt.  This simply enters an infinite loop,
  *         preserving the system state for examination by a debugger.
  * @param  None
  * @retval None
  */
static void __attribute__((unused)) Default_Handler(void)
{
	/* Go into an infinite loop. */
	while (1)
	{
	}
}
static void IntDefaultHandler(void)
{
	/* Go into an infinite loop. */
	while (1)
	{
	}
}


/*********************** (C) COPYRIGHT 2009 Coocox ************END OF FILE*****/
