---------------------------
--                       --
--         main          --
--                       --
---------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity main is
	port(
		CLK	:	in  std_logic;				-- FPGA Clock
		BTN	:	in  std_logic_vector(2 downto 0);	-- Reset

		SW	:	in  std_logic_vector(4 downto 0);	-- Disable motor output
		LD	:	out std_logic_vector(7 downto 0);	-- Index sensor output

		-- SPI0
		JC1	:	in  std_logic;		-- SPI0CLK
		JC2	:	in  std_logic;		-- SPI0SS
		JC3	:	in  std_logic;		-- SPI0MOSI
		JC4	:	out std_logic;		-- SPI0MISO

		-- SPI1
		JC7	:	in  std_logic;		-- SPI1CLK
		JC8	:	in  std_logic;		-- SPI1SS
		JC9	:	in  std_logic;		-- SPI1MOSI
		JC10	:	out std_logic;		-- SPI1MISO

		-- Motor 0
		JB8	:	out std_logic;		-- ENA		Enable Motor 0
		JB3	:	out std_logic;		-- IN2A		Motor 0 pin 1
		JB9	:	out std_logic;		-- IN1A		Motor 0 pin 0

		-- Motor 1
		JB1	:	out std_logic;		-- ENA		Enable Motor 1
		JB2	:	out std_logic;		-- IN1B		Motor 1 pin 0
		JB7	:	out std_logic;		-- IN2B		Motor 1 pin 1

		-- Motor Sensor 0
		JA9	:	in  std_logic;		-- HS1A		Motor 0 sensor 0
		JA3	:	in  std_logic;		-- HS1B		Motor 0 sensor 1

		-- Motor Sensor 1
		JA4	:	in  std_logic;	-- HS2A		Motor 1 sensor 0
		JA10	:	in  std_logic;		-- HS2B		Motor 1 sensor 1

		-- End sencor
		JA1	:	in	std_logic; 	-- End

		-- Index sensor
		JA2	:	in  std_logic;	-- Index 0
		JA8	:	in  std_logic;	-- Index 1

		-- Index Out
		JD1 : out std_logic; -- Index 0
		JD2 : out std_logic -- Index 1
	);
end main;

architecture logic of main is

----------   Components   ----------

	component MotorController is
		generic(
			-- PWM constants
			constant PWMBitWidth		:	positive := 8;	-- 8-bit signed PWM
			constant PWMCLKScale		:	positive := 5;	-- 50MHz / (PWMPrescaler * 2) | (5MHz)
															-- 50MHz / (PWMPrescaler * 2 * 252) | (20KHz)
			-- ENC constants
			constant ENCCLKScale		:	positive := 25;	-- 50MHz / (ENCTimePrescaler * 2) | (1000KHz)
			constant ENCTimeBitWidth	:	positive := 8;	-- 8-bit timer
			constant ENCCountBitWidth	:	positive := 8	-- 8-bit Encoder value
		);
		port(
			RST			:	in  std_logic;	-- Reset
			CLK			:	in  std_logic;	-- Clock

			-- SPI signals
			SPICLK		:	in  std_logic;	-- SPI Clock
			SPISS		:	in  std_logic;	-- SPI Slave Select
			SPIMOSI		:	in  std_logic;	-- SPI MOSI
			SPIMISO		:	out std_logic;	-- SPI MISO

			-- ENC signals
			ENCInput	: 	in  std_logic_vector(1 downto 0);	-- Encoder Input

			ENCOut : out std_logic_vector(3 downto 0);

			-- PWM signals
			PWMOutput	:	out std_logic_vector(1 downto 0);	-- PWM Motor Output

			-- Read/Write test output
			RW		:	out std_logic;	-- Read/Write

			-- Index/Reset/end
			Index : in std_logic;
			Reset_end : in std_logic


		);
	end component;

----------   Signals   ----------

	signal RST0: std_logic;	-- Reset for motorcontroller0
	signal RST1: std_logic;	-- Reset for motorcontroller1

begin
	-- Pass index sensors through
	JD1 <= not JA2;
	JD2 <= not JA8;

	-- Enable H-Bridge
	JB8 <= SW(0) nor SW(1);	-- Enable Motor 0
	JB1 <= SW(0) nor SW(2);	-- Enable Motor 1

	LD(0) <= not JA2;	-- Index 0
	LD(1) <= not JA8;	-- Index 1
	LD(3 downto 2) <= "00";

	-- Reset signal
	RST0 <= BTN(1) or SW(0) or SW(1);
	RST1 <= BTN(2) or SW(0) or SW(2);

	MotorController0: MotorController
	generic map(

		-- PWM constants
		PWMBitWidth => 8,	-- PWM Frequency = 50MHz / (PWMClockPrescaler * 2 * (2 ** PWMBitWidth) - 4)
					-- PWM Frequency -> 50MHz / (5 * 2 * (2 ** 8) - 4) = 20KHz
		PWMCLKScale => 5,	-- Prescaled Clock = 50MHz / (PWMClockPrescaler * 2)
					-- PWMClockPrescaler -> 50MHz / (5 * 2) = 5MHz;

		-- ENC constants
		ENCCLKScale => 1,	-- 50MHz / (ENCTimePrescaler * 2) | (50000KHz)
		ENCTimeBitWidth => 24,	-- 24-bit timer
		ENCCountBitWidth => 8	-- 8-bit Encoder value
	)
	port map(
		RST => RST0,
		CLK => CLK,

		-- SPI signals
		SPICLK => JC1,		-- SPI Clock
		SPISS => JC2,		-- SPI Slave Select
		SPIMOSI => JC3,		-- SPI MOSI
		SPIMISO => JC4,		-- SPI MISO

		-- PWM signals
		PWMOutput(0) => JB9,	-- PWM Motor Output 1
		PWMOutput(1) => JB3,	-- PWM Motor Output 0

		-- Test signal
		--RW => LD(2),

		-- ENC signals
		ENCInput(0) => JA9,	-- Encoder Input 0
		ENCInput(1) => JA3,	-- Encoder Input 1
		ENCOut => LD(7 downto 4),
		Index => not JA2,		-- Index 0
		Reset_end => JA1 		-- EndStop
	);

	MotorController1: MotorController
	generic map(
		-- PWM constants
		PWMBitWidth => 8,	-- PWM Frequency = 50MHz / (PWMClockPrescaler * 2 * (2 ** PWMBitWidth) - 4)
					-- PWM Frequency -> 50MHz / (5 * 2 * (2 ** 8) - 4) = 20KHz
		PWMCLKScale => 5,	-- Prescaled Clock = 50MHz / (PWMClockPrescaler * 2)
					-- PWMClockPrescaler -> 50MHz / (5 * 2) = 5MHz;

		-- ENC constants
		ENCCLKScale => 1,	-- 50MHz / (ENCTimePrescaler * 2) | (50000KHz)
		ENCTimeBitWidth => 24,	-- 24-bit timer
		ENCCountBitWidth => 8	-- 8-bit Encoder value
	)
	port map(
		RST => RST1,
		CLK => CLK,

		-- SPI signals
		SPICLK => JC7,		-- SPI Clock
		SPISS => JC8,		-- SPI Slave Select
		SPIMOSI => JC9,		-- SPI MOSI
		SPIMISO => JC10,	-- SPI MISO

		-- Test signal
		--RW => LD(3),

		-- PWM signals
		PWMOutput(0) => JB2,	-- PWM Motor Output 0
		PWMOutput(1) => JB7,	-- PWM Motor Output 1

		-- ENC signals
		ENCInput(0) => JA4,	-- Encoder Input 0
		ENCInput(1) => JA10,	-- Encoder Input 1
		ENCOut => open,
		Index => not JA8,		-- index 1
		Reset_end => BTN(0)		-- SoftReset
	);
end logic;
