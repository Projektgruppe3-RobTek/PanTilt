#include "SSI2.h"
#include "../../headers/tm4c123gh6pm.h"

static void enable_ssi2_int(void);
static void disable_ssi2_int(void);
__attribute__((unused)) static void enable_rx_int(void);
__attribute__((unused)) static void disable_rx_int(void);
static void disable_tx_int(void);
static void enable_tx_int(void);
static void ssi2_rx_isr(void);
static void ssi2_tx_isr(void);


//As we use the 8 bit system buffers to store 16 bit data, there may be problems if they become full, and we may get 4 bit skew.
//Should maybe be fixed sometime. Make 16 bit ringbuffers?
//When we push to the buffer, push MSB, then LSB.
//When we pop, pop LSB, then MSB.
static INT8U ssi2_buffer_in;
static INT8U ssi2_buffer_out;



static void disable_ssi2_int(void)
{
	NVIC_DIS1_R |= (0x01 << (INT_SSI2-(16+32)));
}
static void enable_ssi2_int(void)
{
	NVIC_EN1_R |= (0x01 << (INT_SSI2-(16+32)));
}

static void enable_rx_int(void)
{
	SSI2_IM_R |= SSI_IM_RXIM;
}
static void disable_rx_int(void)
{
	SSI2_IM_R &= ~SSI_IM_RXIM;
}
static void disable_tx_int(void)
{
		SSI2_IM_R &= ~SSI_IM_TXIM;
}
static void enable_tx_int(void)
{
		SSI2_IM_R |= SSI_IM_TXIM;
}


void ssi2_in_clear(void)
{
	sys_ringbuf_uchar_release(ssi2_buffer_in);
	ssi2_buffer_in = sys_ringbuf_uchar_request();
}

void ssi2_out_clear(void)
{
	sys_ringbuf_uchar_release(ssi2_buffer_out);
	ssi2_buffer_out = sys_ringbuf_uchar_request();
}


INT16U ssi2_in_16bit(void)
{
	while(ssi2_data_available() == 0 && !(SSI2_SR_R & SSI_SR_RNE)); //wait while both ringbuffer and FIFO is empty

	//Fill ringbuffer from fifo if available
	if ( SSI2_SR_R & SSI_SR_RNE)
		ssi2_rx_isr();

	disable_ssi2_int();
	INT16U returnval = sys_ringbuf_uchar_pop(ssi2_buffer_in); //LSB
	returnval += sys_ringbuf_uchar_pop(ssi2_buffer_in) << 8; //MSB
	enable_ssi2_int();
	return returnval;
}


void ssi2_out_16bit(INT16U data)
{
	if((SSI2_SR_R & SSI_SR_TNF) && sys_ringbuf_uchar_size(ssi2_buffer_out) == 0)  //check if transmit fifo is full, if not, and buffer is empty, just push to FIFO
		SSI2_DR_R = data & 0xFFFF;
	else
	{
		while(sys_ringbuf_uchar_full(ssi2_buffer_out));

		disable_ssi2_int();
		INT8U first_8_bit = data & 0x00FF;
		INT8U last_8_bit = (data & 0xFF00) >> 8;
		sys_ringbuf_uchar_push(ssi2_buffer_out, first_8_bit);
		sys_ringbuf_uchar_push(ssi2_buffer_out, last_8_bit);
		enable_ssi2_int();
		enable_tx_int();
	}
}


RBUF_INDEX_TYPE ssi2_out_space_left(void)
{
	return (sys_ringbuf_uchar_max(ssi2_buffer_out) - sys_ringbuf_uchar_size(ssi2_buffer_out)) / 2 ; //Divide by 2 as we use 2 places per 16 bit.
}

RBUF_INDEX_TYPE ssi2_in_space_left(void)
{
	return (sys_ringbuf_uchar_max(ssi2_buffer_in) - sys_ringbuf_uchar_size(ssi2_buffer_in)) / 2; //Divide by 2 as we use 2 places per 16 bit.
}

RBUF_INDEX_TYPE ssi2_data_available(void)
{
	//Fill ringbuffer from fifo if available
	if ( SSI2_SR_R & SSI_SR_RNE) //Recieve Fifo not empty
		ssi2_rx_isr();

	return sys_ringbuf_uchar_size(ssi2_buffer_in) / 2; //Divide by 2 as we use 2 places per 16 bit.
}


void ssi2_isr(void)
{
	//check if we have been interrupted for recieve or transmit
	if(SSI2_MIS_R & SSI_MIS_TXMIS)
		ssi2_tx_isr();
	if (SSI2_MIS_R & SSI_MIS_RXMIS)
		ssi2_rx_isr();
}

static void ssi2_tx_isr(void)
{
	//fill FIFO from ringbuffer
	while((SSI2_SR_R & SSI_SR_TNF) && sys_ringbuf_uchar_size(ssi2_buffer_out)) //while not full and buffer not empty
	{
		disable_ssi2_int();
		INT16U data = sys_ringbuf_uchar_pop(ssi2_buffer_out); //LSB
		data += sys_ringbuf_uchar_pop(ssi2_buffer_out) << 8; //MSB
		SSI2_DR_R = data;
		enable_ssi2_int();
	}
	//if buffer is empty, disable tx interrupt. Will be enabled again next time there is something in the buffer.
	if(!sys_ringbuf_uchar_size(ssi2_buffer_out)) disable_tx_int();
	//we can't clear SSI interrupt? Does it do it itself? I don't think so.
	///////////////////////////////////////////////////////////////////////
	/////////////////////////This is probably a problem////////////////////
	///////////////////////////////////////////////////////////////////////
}

static void ssi2_rx_isr(void)
{
	//fill ringbuffer from FIFO
	while(SSI2_SR_R & SSI_SR_RNE ) //while FIFO not empty.
	{
		INT16U in_data = SSI2_DR_R;
		if (!sys_ringbuf_uchar_full(ssi2_buffer_in))	//discarded if buffer is full.
		{
			disable_ssi2_int();
			INT8U first_8_bit = in_data & 0x00FF;
			INT8U last_8_bit = (in_data & 0xFF00) >> 8;
			sys_ringbuf_uchar_push(ssi2_buffer_in, first_8_bit);
			sys_ringbuf_uchar_push(ssi2_buffer_in, last_8_bit);
			enable_ssi2_int();
		}
	}
}


void setup_ssi2()
{
	//Setup buffers//
	ssi2_buffer_in = sys_ringbuf_uchar_request(); //we use 8 bit buffers, and use two values each time
	ssi2_buffer_out = sys_ringbuf_uchar_request();

	//Setup SSI//
	// Step 1
	SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R2;	// enable and provide a clock to SSI module 2 in Run mode.

	// Step 2
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
	//Wait a bit
	__asm__("NOP");
	// Step 3
	GPIO_PORTB_AFSEL_R |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);	// enable alternative functions

	// Step 4
	GPIO_PORTB_PCTL_R &=  ~(GPIO_PCTL_PB4_M | GPIO_PCTL_PB5_M | GPIO_PCTL_PB6_M | GPIO_PCTL_PB7_M);
	GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB4_SSI2CLK | GPIO_PCTL_PB5_SSI2FSS | GPIO_PCTL_PB6_SSI2RX | GPIO_PCTL_PB7_SSI2TX;

	// Step 5
	GPIO_PORTB_DEN_R |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);             // enable digital I/O on PB4-7

	// disable ssi
	SSI2_CR1_R &= ~SSI_CR1_SSE;

	// set master
	SSI2_CR1_R &=~SSI_CR1_MS;

	// use 16 MHz clock
	SSI2_CC_R = SSI_CC_CS_PIOSC;

	// set prescaler
	SSI2_CR0_R &= ~SSI_CR0_SCR_M;

	SSI2_CR0_R |= (SCR << SSI_CR0_SCR_S);		//divisors is defined in SSI.h
	SSI2_CPSR_R = CPSDVSR;

	// data is captured on the second clock edge transition
	SSI2_CR0_R |= SSI_CR0_SPH;	// SPH = 1

	// SSI2_CLK steady state high
	SSI2_CR0_R |= SSI_CR0_SPO;	// SPO = 1

	//Select Freescale SPI Format
	SSI2_CR0_R &= ~SSI_CR0_FRF_MOTO;
	SSI2_CR0_R |= SSI_CR0_FRF_MOTO;

	//Select 16 bit datasize
	SSI2_CR0_R &= ~SSI_CR0_DSS_M;
	SSI2_CR0_R |= SSI_CR0_DSS_16;

	// Connect MISO to MOSI
	//SSI2_CR1_R |= SSI_CR1_LBM;			// Only for test !!!!!!!!!!!!


	//Enable SSI
	SSI2_CR1_R |= SSI_CR1_SSE;

	//Interrupt on recieve and transmit, half full/empty
	SSI2_IM_R |= SSI_IM_RXIM | SSI_IM_TXIM;

	enable_ssi2_int();

}
