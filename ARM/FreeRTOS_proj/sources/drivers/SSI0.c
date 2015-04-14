#include "SSI0.h"
#include "../../headers/tm4c123gh6pm.h"

static void enable_ssi0_int(void);
static void disable_ssi0_int(void);
__attribute__((unused)) static void enable_rx_int(void);
__attribute__((unused)) static void disable_rx_int(void);
static void disable_tx_int(void);
static void enable_tx_int(void);
static void ssi0_rx_isr(void);
static void ssi0_tx_isr(void);


//As we use the 8 bit system buffers to store 16 bit data, there may be problems if they become full, and we may get 4 bit skew.
//Should maybe be fixed sometime. Make 16 bit ringbuffers?
//When we push to the buffer, push MSB, then LSB.
//When we pop, pop LSB, then MSB.
static INT8U ssi0_buffer_in;
static INT8U ssi0_buffer_out;



static void disable_ssi0_int(void)
{
	NVIC_DIS0_R |= (0x01 << (INT_SSI0-16));
}
static void enable_ssi0_int(void)
{
	NVIC_EN0_R |= (0x01 << (INT_SSI0-16));
}

static void enable_rx_int(void)
{
	SSI0_IM_R |= SSI_IM_RXIM;
}
static void disable_rx_int(void)
{
	SSI0_IM_R &= ~SSI_IM_RXIM;
}
static void disable_tx_int(void)
{
		SSI0_IM_R &= ~SSI_IM_TXIM;
}
static void enable_tx_int(void)
{
		SSI0_IM_R |= SSI_IM_TXIM;
}


void ssi0_in_clear(void)
{
	sys_ringbuf_uchar_release(ssi0_buffer_in);
	ssi0_buffer_in = sys_ringbuf_uchar_request();
}

void ssi0_out_clear(void)
{
	sys_ringbuf_uchar_release(ssi0_buffer_out);
	ssi0_buffer_out = sys_ringbuf_uchar_request();
}


INT16U ssi0_in_16bit(void)
{
	while(ssi0_data_available() == 0 && !(SSI0_SR_R & SSI_SR_RNE)); //wait while both ringbuffer and FIFO is empty

	//Fill ringbuffer from fifo if available
	if ( SSI0_SR_R & SSI_SR_RNE)
		ssi0_rx_isr();

	disable_ssi0_int();
	INT16U returnval = sys_ringbuf_uchar_pop(ssi0_buffer_in); //LSB
	returnval += sys_ringbuf_uchar_pop(ssi0_buffer_in) << 8; //MSB
	enable_ssi0_int();
	return returnval;
}


void ssi0_out_16bit(INT16U data)
{
	if((SSI0_SR_R & SSI_SR_TNF) && sys_ringbuf_uchar_size(ssi0_buffer_out) == 0)  //check if transmit fifo is full, if not, and buffer is empty, just push to FIFO
		SSI0_DR_R = data & 0xFFFF;
	else
	{
		while(sys_ringbuf_uchar_full(ssi0_buffer_out));

		disable_ssi0_int();
		INT8U first_8_bit = data & 0x00FF;
		INT8U last_8_bit = (data & 0xFF00) >> 8;
		sys_ringbuf_uchar_push(ssi0_buffer_out, first_8_bit);
		sys_ringbuf_uchar_push(ssi0_buffer_out, last_8_bit);
		enable_ssi0_int();
		enable_tx_int();
	}
}


RBUF_INDEX_TYPE ssi0_out_space_left(void)
{
	return (sys_ringbuf_uchar_max(ssi0_buffer_out) - sys_ringbuf_uchar_size(ssi0_buffer_out)) / 2 ; //Divide by 2 as we use 2 places per 16 bit.
}

RBUF_INDEX_TYPE ssi0_in_space_left(void)
{
	return (sys_ringbuf_uchar_max(ssi0_buffer_in) - sys_ringbuf_uchar_size(ssi0_buffer_in)) / 2; //Divide by 2 as we use 2 places per 16 bit.
}

RBUF_INDEX_TYPE ssi0_data_available(void)
{
	//Fill ringbuffer from fifo if available
	if ( SSI0_SR_R & SSI_SR_RNE) //Recieve Fifo not empty
		ssi0_rx_isr();

	return sys_ringbuf_uchar_size(ssi0_buffer_in) / 2; //Divide by 2 as we use 2 places per 16 bit.
}


void ssi0_isr(void)
{
	//check if we have been interrupted for recieve or transmit
	if(SSI0_MIS_R & SSI_MIS_TXMIS)
		ssi0_tx_isr();
	if (SSI0_MIS_R & SSI_MIS_RXMIS)
		ssi0_rx_isr();
}

static void ssi0_tx_isr(void)
{
	//fill FIFO from ringbuffer
	while((SSI0_SR_R & SSI_SR_TNF) && sys_ringbuf_uchar_size(ssi0_buffer_out)) //while not full and buffer not empty
	{
		disable_ssi0_int();
		INT16U data = sys_ringbuf_uchar_pop(ssi0_buffer_out); //LSB
		data += sys_ringbuf_uchar_pop(ssi0_buffer_out) << 8; //MSB
		SSI0_DR_R = data;
		enable_ssi0_int();
	}
	//if buffer is empty, disable tx interrupt. Will be enabled again next time there is something in the buffer.
	if(!sys_ringbuf_uchar_size(ssi0_buffer_out)) disable_tx_int();
	//we can't clear SSI interrupt? Does it do it itself? I don't think so.
	///////////////////////////////////////////////////////////////////////
	/////////////////////////This is probably a problem////////////////////
	///////////////////////////////////////////////////////////////////////
}

static void ssi0_rx_isr(void)
{
	//fill ringbuffer from FIFO
	while(SSI0_SR_R & SSI_SR_RNE ) //while FIFO not empty.
	{
		INT16U in_data = SSI0_DR_R;
		if (!sys_ringbuf_uchar_full(ssi0_buffer_in))	//discarded if buffer is full.
		{
			disable_ssi0_int();
			INT8U first_8_bit = in_data & 0x00FF;
			INT8U last_8_bit = (in_data & 0xFF00) >> 8;
			sys_ringbuf_uchar_push(ssi0_buffer_in, first_8_bit);
			sys_ringbuf_uchar_push(ssi0_buffer_in, last_8_bit);
			enable_ssi0_int();
		}
	}
}


void setup_ssi0()
{
	//Setup buffers//
	ssi0_buffer_in = sys_ringbuf_uchar_request(); //we use 8 bit buffers, and use two values each time
	ssi0_buffer_out = sys_ringbuf_uchar_request();

	//Setup SSI//
	// Step 1
	SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R0;	// enable and provide a clock to SSI module 0 in Run mode.

	// Step 2
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_SSI0;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	//Wait a bit
	__asm__("NOP");
	// Step 3
	GPIO_PORTA_AFSEL_R |= (1 << 2) | (1<<3) | (1<<4) | (1<<5);	// enable alternative functions

	// Step 4
	GPIO_PORTA_PCTL_R &=  ~(GPIO_PCTL_PA2_M | GPIO_PCTL_PA3_M | GPIO_PCTL_PA4_M | GPIO_PCTL_PA5_M);
	GPIO_PORTA_PCTL_R |= GPIO_PCTL_PA2_SSI0CLK | GPIO_PCTL_PA3_SSI0FSS | GPIO_PCTL_PA4_SSI0RX | GPIO_PCTL_PA5_SSI0TX;

	// Step 5
	GPIO_PORTA_DEN_R |= (1 << 2) | (1<<3) | (1<<4) | (1<<5);;             // enable digital I/O on PA2-5

	// disable ssi
	SSI0_CR1_R &= ~SSI_CR1_SSE;

	// set master
	SSI0_CR1_R &=~SSI_CR1_MS;

	// use 16 MHz clock
	SSI0_CC_R = SSI_CC_CS_PIOSC;

	// set prescaler
	SSI0_CR0_R &= ~SSI_CR0_SCR_M;
	//SSI0_CR0_R |= (1 << SSI_CR0_SCR_S);
	//SSI0_CPSR_R = 2;
	SSI0_CR0_R |= (SCR << SSI_CR0_SCR_S);		//divisors is defined in SSI.h.
	SSI0_CPSR_R = CPSDVSR;

	// data is captured on the second clock edge transition
	SSI0_CR0_R |= SSI_CR0_SPH;	// SPH = 1

	// SSI0_CLK steady state high
	SSI0_CR0_R |= SSI_CR0_SPO;	// SPO = 1

	//Select Freescale SPI Format
	SSI0_CR0_R &= ~SSI_CR0_FRF_MOTO;
	SSI0_CR0_R |= SSI_CR0_FRF_MOTO;

	//Select 16 bit datasize
	SSI0_CR0_R &= ~SSI_CR0_DSS_M;
	SSI0_CR0_R |= SSI_CR0_DSS_16;

	// Connect MISO to MOSI
	//SSI0_CR1_R |= SSI_CR1_LBM;			// Only for test !!!!!!!!!!!!


	//Enable SSI
	SSI0_CR1_R |= SSI_CR1_SSE;

	//Interrupt on recieve and transmit, half full/empty
	SSI0_IM_R |= SSI_IM_RXIM | SSI_IM_TXIM;

	enable_ssi0_int();

	////////////////////
	// SSI0 registers //
	////////////////////

	// Read/Write
	//SSI0_CR0_R |= (0 << DSS);	// number of data bit - 1 (0x3 - 0xF)
	//SSI0_CR0_R |= (0 << FRF);	// frame format (0 = Freescale SPI, 1 = Texas Instrument SS, 2 = MICROWIRE)
	//SSI0_CR0_R |= SSI_CR0_SPO;	// Steady state slave select (0 = Low, 1 = High)
	//SSI0_CR0_R |= (0 << SPH);7	// capture on first edge or second edge (0 = first, 1 = second)
	//SSI0_CR0_R |= (0 << SCR);	// Bit rate (BR = SysClk / (CPSDVSR * (1 + SCR))) where
					// PSDVSR is an even value from 2-254 programmed in the SSI0_CPSR_R register
					// and SCR is a value from 0-255 in the SSI0_CR0_R

	// Read/Write
	//SSI0_CR1_R |= (0 << LBM);	// Loop back (0 = normal operation, 1 = local output goes to local input)
	//SSI0_CR1_R |= (0 << SSE);	// Enable (0 = disabled, 1 = enabled)
	//SSI0_CR1_R |= (0 << MS);	// Master / Slave (0 = master, 1 = slave)
	//SSI0_CR1_R |= (0 << EOT);	// look at TXRIS in SSI0_RIS_R

	// Read/Write
	//SSI0_DR_R |= (0);		// Read / write data (4 - 16 bits)

	/* Read only
	SSI0_SR_R |= (0 << TFE);	// transmit FIFO full (0 = not full, 1 = full)
	SSI0_SR_R |= (0 << TNF);	// transmit FIFO not full (0 = full, 1 not full)
	SSI0_SR_R |= (0 << RNE);	// receive FIFO not empty (0 = empty, 1 not empty)
	SSI0_SR_R |= (0 << RFF);	// receive FIFO full (0 = not full, 1 = full)
	SSI0_SR_R |= (0 << BSY);	// busy (0 = idle, 1 = transmitting or receiving or transmit FIFO full)
	*/

	// Read/Write
	//SSI0_CPSR_R |= (0 << PSDVSR);	// Bit rate (BR = SysClk / (CPSDVSR * (1 + SCR))) where
					// PSDVSR is an even value from 2-254 programmed in the SSI0_CPSR_R register
					// and SCR is a value from 0-255 in the SSI0_CR0_R

	// Read/Write
	//SSI0_IM_R |= (0 << RORIM);	// receive FIFO overrun interrupt not masked (0 = masked, 1 = not masked)
	//SSI0_IM_R |= (0 << RTIM);	// receive FIFO time-out interrupt not masked (0 = masked, 1 = not masked)
	//SSI0_IM_R |= (0 << RXIM);	// receive FIFO interrupt not masked (0 = masked, 1 = not masked)
	//SSI0_IM_R |= (0 << TXIM);	// transmit FIFO interrupt not masked (0 = masked, 1 = not masked)

	/* Read only
	SSI0_RIS_R | = (0 << RORRIS);	// receive overrun interrupt status (0 = no interrupt, 1 = overflowed)
	SSI0_RIS_R | = (0 << RTRIS);	// receive time-out raw interrupt status (0 = no interrupt, 1 = time-out)
	SSI0_RIS_R | = (0 << RXRIS);	// receive FIFO raw interrupt status (0 = no interrupt, 1 = receive FIFO is half full)
	SSI0_RIS_R | = (0 << TXRIS);	// transmit FIFO raw interrupt status (0 = no interrupt,
					// 1 = transmit FIFO is half empty or less (if EOT in SSI0_CR1_R is set)
					// 1 = transmit FIFO is empty (if EOT in SSI0_CR1_R is clear))
	*/

	/* Read only
	SSI0_MIS_R |= (0 << RORMIS);	// receive overrun masked interrupt status (0 = no unmasked interrupt has accurred,
					// 1 = an unmasked interrupt has accurred)
	SSI0_MIS_R |= (0 << RTMIS);	// receive time-out masked interrupt status (0 = no unmasked interrupt has accurred,
					// 1 = an unmasked interrupt has accurred)
	SSI0_MIS_R |= (0 << RXMIS);	// receive FIFO masked interrupt status (0 = no unmasked interrupt has accurred,
					// 1 = an unmasked interrupt has accurred)
	SSI0_MIS_R |= (0 << TXMIS);	// transmit FIFO masked interrupt status (0 = no unmasked interrupt has accurred,
					// 1 = an unmasked interrupt has accurred)
	*/

	// W1C ??
	//SSI0_ICR_R |= (0 << RORIC);	// write high to clear the RORRIS bit in SSI0_RIS_R and the RORMIS in SSI0_MIS_R
	//SSI0_ICR_R |= (0 << RTIC);	// write high to clear the RTRIS bit in SSI0_RIS_R and the RTMIS in SSI0_MIS_R

	// Read/Write
	//SSI0_DMACTL_R |= (0 << RXDMAE);	// enable MICRO-DMA for receive FIFO (0 = disabled, 1 = enabled)
	//SSI0_DMACTL_R |= (0 << TXDMAE); // enable MICRO-DMA for transmit FIFO (0 = disabled, 1 = enabled)

	// Read/Write
	//SSI0_CC_R |= (0);		// clock source (0 = system clock, 5 = PIOSC)
}
