#include "UART.h"
static void uart0_tx_isr(void);
static void uart0_rx_isr(void);


static INT8U buffer_in;
static INT8U buffer_out;

static void disable_uart0_int(void);
static void enable_uart0_int(void);

static void disable_uart0_int(void)
{
  NVIC_DIS0_R |= (0x01 << (INT_UART0-16));
}
static void enable_uart0_int(void)
{
  NVIC_EN0_R |= (0x01 << (INT_UART0-16));
}

void uart0_out_string(char *string)
{
	INT16U index = 0;
	while(string[index])
	{
		uart0_out_char(string[index++]);
	}
}

void uart0_in_clear(void)
{
	sys_ringbuf_uchar_release(buffer_in);
	buffer_in = sys_ringbuf_uchar_request();
}

void uart0_out_clear(void)
{
	sys_ringbuf_uchar_release(buffer_out);
	buffer_out = sys_ringbuf_uchar_request();
}

char uart0_in_char(void)
{
  while(uart0_data_available() == 0 && (UART0_FR_R & UART_FR_RXFE)); //wait while both ringbuffer and FIFO is empty

	//Fill ringbuffer from fifo if available
	if ( !(UART0_FR_R & UART_FR_RXFE))
		uart0_rx_isr();

	disable_uart0_int();
	char returnval = sys_ringbuf_uchar_pop(buffer_in);
	enable_uart0_int();
  return returnval;
}

void uart0_out_char(char data){
	if(!(UART0_FR_R & UART_FR_TXFF) && sys_ringbuf_uchar_size(buffer_out) == 0)  //check if transmit fifo is full, if not, and buffer is empty, just push to FIFO
	  UART0_DR_R = data;
	else
	{
		while(sys_ringbuf_uchar_full(buffer_out));

		disable_uart0_int();
		sys_ringbuf_uchar_push(buffer_out, data);
		enable_uart0_int();
	}

}
RBUF_INDEX_TYPE uart0_in_space_left(void)
{
	return sys_ringbuf_uchar_max(buffer_in) - sys_ringbuf_uchar_size(buffer_in);
}

RBUF_INDEX_TYPE uart0_out_space_left(void)
{
	return sys_ringbuf_uchar_max(buffer_out) - sys_ringbuf_uchar_size(buffer_out);
}

RBUF_INDEX_TYPE uart0_data_available(void)
{
	//Fill ringbuffer from fifo if available
	if ( !(UART0_FR_R & UART_FR_RXFE))
		uart0_rx_isr(); //

	return sys_ringbuf_uchar_size(buffer_in);
}

void setup_uart0(void){
	//setup buffers
	buffer_in = sys_ringbuf_uchar_request();
	buffer_out = sys_ringbuf_uchar_request();
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0; // activate UART0
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // activate port A
  __asm__("NOP");
  __asm__("NOP");
  UART0_CTL_R &= ~UART_CTL_UARTEN;      // disable UART
  UART0_IBRD_R = IBRD;
  UART0_FBRD_R = FBRD;
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  UART0_CTL_R |= UART_CTL_UARTEN;       // enable UART
  GPIO_PORTA_AFSEL_R |= 0x01 | 0x02;           // enable alt funct on PA1-0
  GPIO_PORTA_DEN_R |= 0x01 | 0x02;             // enable digital I/O on PA1-0

                                        // configure PA1-0 as UART
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & ~(GPIO_PCTL_PA0_M | GPIO_PCTL_PA1_M)) |
  										GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX;

  GPIO_PORTA_AMSEL_R &= ~(0x01 | 0x02);          // disable analog functionality on PA

  //Interrupt on recieve and transmit
  UART0_IM_R |= UART_IM_RXIM | UART_IM_TXIM;

  //interrupt on 4/8 full FIFO recieve
  UART0_IFLS_R = (UART0_IFLS_R &  ~UART_IFLS_RX_M) | UART_IFLS_RX4_8 ;
  //Interrupt on 7/8 empty FIFO transmit.
  UART0_IFLS_R = (UART0_IFLS_R &  ~UART_IFLS_TX_M) | UART_IFLS_TX7_8 ;

  //Enable uart interrupts
  enable_uart0_int();

}

void uart0_isr(void)
{
	//check if we have been interrupted for recieve or transmit
	if(UART0_MIS_R & UART_MIS_TXMIS)
		uart0_tx_isr();
	if (UART0_MIS_R & UART_MIS_RXMIS)
		uart0_rx_isr();
}

static void uart0_tx_isr(void)
{
	//fill FIFO from ringbuffer
	while( (!(UART0_FR_R & UART_FR_TXFF)) && sys_ringbuf_uchar_size(buffer_out)) //while not full and buffer not empty
	{
		disable_uart0_int();
		UART0_DR_R = sys_ringbuf_uchar_pop(buffer_out);
		enable_uart0_int();
	}
	//clear interrupt
	UART0_ICR_R |= UART_ICR_TXIC;
}

static void uart0_rx_isr(void)
{
	//fill ringbuffer from FIFO
	INT8U inchar;
	while( !(UART0_FR_R & UART_FR_RXFE) ) //while FIFO not empty.
	{
		inchar = (INT8U)(UART0_DR_R&0xFF);
		if (!sys_ringbuf_uchar_full(buffer_in))	//char is discarded if buffer is full.
		{
			disable_uart0_int();
			sys_ringbuf_uchar_push(buffer_in, inchar);
			enable_uart0_int();
		}
	}
}
