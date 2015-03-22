---------------------------
--                       --
--         main          --
--                       --
---------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity main is
	port(
		BTN	:	in  std_logic_vector(0 downto 0);	-- Reset
		CLK	:	in  std_logic;				-- FPGA Clock
		
		SW	:	in  std_logic_vector(2 downto 0);	-- Disable motor output
		LD	:	out std_logic_vector(1 downto 0);	-- Index sensor output
		
		-- SPI0
		JC1	:	in  std_logic;	-- SPI0CLK
		JC2	:	in  std_logic;	-- SPI0SS
		JC3	:	in  std_logic;	-- SPI0MOSI
		JC4	:	out std_logic;	-- SPI0MISO
		
		-- SPI1
		JC7	:	in  std_logic;	-- SPI1CLK
		JC8	:	in  std_logic;	-- SPI1SS
		JC9	:	in  std_logic;	-- SPI1MOSI
		JC10	:	out std_logic;	-- SPI1MISO
		
		-- Motor 0
		JB8	:	out std_logic;	-- ENA		Enable Motor 0
		JB3	:	out std_logic;	-- IN1A		Motor 0 pin 0
		JB9	:	out std_logic;	-- IN2A		Motor 0 pin 1
		
		-- Motor 1
		JB1	:	out std_logic;	-- ENA		Enable Motor 1
		JB2	:	out std_logic;	-- IN1B		Motor 1 pin 0
		JB7	:	out std_logic;	-- IN2B		Motor 1 pin 1
		
		-- Motor Sensor 0
		JA9	:	in  std_logic;	-- HS1A		Motor 0 sensor 0
		JA3	:	in  std_logic;	-- HS1B		Motor 0 sensor 1
		
		-- Motor Sensor 1
		JA4	:	in  std_logic;	-- HS2A		Motor 1 sensor 0
		JA10	:	in  std_logic;	-- HS2B		Motor 1 sensor 1
		
		-- Index sensor
		JA2	:	in  std_logic;	-- Index 0
		JA8	:	in  std_logic	-- Index 1
	);
end main;

architecture logic of main is
	
----------   Components   ----------
	
	component MotorController is
		generic(
			-- PWM constants
			constant PWMBitWidth: positive := 8;		-- 8-bit signed PWM
			constant PWMPrescaler: positive := 5;		-- 50MHz / (PWMPrescaler * 2) | (5MHz)
										-- 50MHz / (PWMPrescaler * 2 * 252) | (20KHz)
			-- ENC constants
			constant ENCTimeBitWidth: positive := 8;	-- 8-bit timer
			constant ENCTimePrescaler: positive := 25;	-- 50MHz / (ENCTimePrescaler * 2) | (1000KHz)
			constant ENCValBitWidth: positive := 8	-- 8-bit Encoder value
		);
		port(
			-- 
			RST: in std_logic;	-- Reset
			CLK: in std_logic;	-- Clock
		
			-- SPI signals
			SPICLK: in std_logic;	-- SPI Clock
			SPISS: in std_logic;	-- SPI Slave Select
			SPIMOSI: in std_logic;	-- SPI MOSI
			SPIMISO: out std_logic;	-- SPI MISO

			-- PWM signals
			PWMOutput: out std_logic_vector(1 downto 0);	-- PWM Motor Output
			
			-- ENC signals
			ENCInput: in std_logic_vector(1 downto 0)	-- Encoder Input
		);
	end component;
	
begin
	
	-- Enable H-Bridge
	JB8 <= SW(0) nor SW(1);	-- Enable Motor 0
	JB1 <= SW(0) nor SW(2);	-- Enable Motor 1
	
	LD(0) <= JA2;	-- Index 0
	LD(1) <= JA8;	-- Index 1
	
	MotorController0: MotorController
	generic map(
		-- PWM constants
		PWMBitWidth => 8,	-- PWM Frequency = 50MHz / (PWMClockPrescaler * 2 * (2 ** PWMBitWidth) - 4)
					-- PWM Frequency -> 50MHz / (5 * 2 * (2 ** 8) - 4) = 20KHz
		PWMPrescaler => 5,	-- Prescaled Clock = 50MHz / (PWMClockPrescaler * 2)
					-- PWMClockPrescaler -> 50MHz / (5 * 2) = 5MHz;
		
		-- ENC constants
		ENCTimeBitWidth => 8,	-- 8-bit timer
		ENCTimePrescaler => 25,	-- 50MHz / (ENCTimePrescaler * 2) | (1000KHz)
		ENCValBitWidth => 8	-- 8-bit Encoder value
	)
	port map(
		RST => BTN(0),
		CLK => CLK,
		
		-- SPI signals
		SPICLK => JC1,		-- SPI Clock
		SPISS => JC2,		-- SPI Slave Select
		SPIMOSI => JC3,		-- SPI MOSI
		SPIMISO => JC4,		-- SPI MISO
		
		-- PWM signals
		PWMOutput(0) => JB3,	-- PWM Motor Output 0
		PWMOutput(1) => JB9,	-- PWM Motor Output 1
		
		-- ENC signals
		ENCInput(0) => JA9,	-- Encoder Input 0
		ENCInput(1) => JA3	-- Encoder Input 1
	);
	
	MotorController1: MotorController
	generic map(
		-- PWM constants
		PWMBitWidth => 8,	-- PWM Frequency = 50MHz / (PWMClockPrescaler * 2 * (2 ** PWMBitWidth) - 4)
					-- PWM Frequency -> 50MHz / (5 * 2 * (2 ** 8) - 4) = 20KHz
		PWMPrescaler => 5,	-- Prescaled Clock = 50MHz / (PWMClockPrescaler * 2)
					-- PWMClockPrescaler -> 50MHz / (5 * 2) = 5MHz;
		
		-- ENC constants
		ENCTimeBitWidth => 8,	-- 8-bit timer
		ENCTimePrescaler => 25,	-- 50MHz / (ENCTimePrescaler * 2) | (1000KHz)
		ENCValBitWidth => 8	-- 8-bit Encoder value
	)
	port map(
		RST => BTN(0),
		CLK => CLK,
		
		-- SPI signals
		SPICLK => JC7,		-- SPI Clock
		SPISS => JC8,		-- SPI Slave Select
		SPIMOSI => JC9,		-- SPI MOSI
		SPIMISO => JC10,	-- SPI MISO
		
		-- PWM signals
		PWMOutput(0) => JB2,	-- PWM Motor Output 0
		PWMOutput(1) => JB7,	-- PWM Motor Output 1
		
		-- ENC signals
		ENCInput(0) => JA4,	-- Encoder Input 0
		ENCInput(1) => JA10	-- Encoder Input 1
	);
	
end logic;
