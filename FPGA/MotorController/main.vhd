------------
--  main  --
------------
library ieee;
use ieee.std_logic_1164.all;

entity main is
	port(
		-- 
		BTN: in std_logic_vector(0 downto 0);	-- Reset button
		Clock: in std_logic;			-- FPGA Clock
		LD: out std_logic_vector(5 downto 0);	-- Sensor Output
		
		-- Sensor
		JA2: in std_logic;   -- Index0		HallSensor 0
		JA3: in std_logic;   -- HS1B		Motor 0 sensor 1
		JA4: in std_logic;   -- HS2A		Motor 1 sensor 0
		JA8: in std_logic;   -- Index1		HallSensor 1
		JA9: in std_logic;   -- HS1A		Motor 0 sensor 0
		JA10: in std_logic;  -- HS2B		Motor 1 sensor 1
		
		-- Motor
		JB1: out std_logic;  -- ENB		Enable motor 1
		JB2: out std_logic;  -- IN1B		Motor 1 pin 0
		JB3: out std_logic;  -- IN1A		Motor 0 pin 0
		JB7: out std_logic;  -- IN2B		Motor 1 pin 1
		JB8: out std_logic;  -- ENA		Enable Motor 0
		JB9: out std_logic;  -- IN2A		Motor 0 pin 1
		
		-- SPI0
		JC1: in std_logic;  -- SPI0CLK
		JC2: in std_logic;  -- SPI0SS
		JC3: in std_logic;  -- SPI0MOSI
		JC4: out std_logic;  -- SPI0MISO
		
		-- SPI1
		JC7: in std_logic;  -- SPI1CLK
		JC8: in std_logic;  -- SPI1SS
		JC9: in std_logic;  -- SPI1MOSI
		JC10: out std_logic  -- SPI1MISO
	);
end main;

architecture behav of main is
	
	component MotorController is
		generic(
			constant ControlerPWMBitWidth: positive := 8;		-- 8-bit signed PWM
			constant ControlerPWMPrescaler: positive := 25;		-- 50MHz / (PWMPrescaler * 2) | (1000KHz)
			constant ControlerENCTimeBitWidth: positive := 8;		-- 8-bit timer
			constant ControlerENCTimePrescaler: positive := 25;	-- 50MHz / (ENCTimePrescaler * 2) | (1000KHz)
			constant ControlerENCValBitWidth: positive := 8		-- 8-bit Encoder value
		);
		port(
			Reset: in std_logic;	-- Reset
			Clock: in std_logic;	-- Clock
		
			SPICLK: in std_logic;	-- SPI Clock
			SPISS: in std_logic;	-- SPI Slave Select
			SPIMOSI: in std_logic;	-- SPI MOSI
			SPIMISO: out std_logic;	-- SPI MISO
		
			PWMOutput: out std_logic_vector(1 downto 0);	-- PWM Motor Output
			PWMEN: out std_logic;				-- H-Bridge Enable
		
			ENCInput: in std_logic_vector(1 downto 0)	-- Encoder Input
		);
	end component;
	
	signaL SPISS1, SPISS2, SPIMISO1, SPIMISO2: std_logic;
	
	begin
	
	-- Sensor Output
	LD(0) <= JA2;	-- Index0	HallSensor 0
	LD(1) <= JA8;	-- Index1	HallSensor 1
	LD(2) <= JA2;	-- HS1A		Motor 0 sensor 0
	LD(3) <= JA8;	-- HS1B		Motor 0 sensor 1
	LD(4) <= JA2;	-- HS2A		Motor 1 sensor 0
	LD(5) <= JA8;	-- HS2B		Motor 1 sensor 1
	
	Motor1: MotorController
	generic map(
		ControlerPWMBitWidth => 8,		-- 8-bit signed PWM
		ControlerPWMPrescaler => 25,		-- 50MHz / (PWMPrescaler * 2) | (1000KHz)
		ControlerENCTimeBitWidth => 8,		-- 8-bit timer
		ControlerENCTimePrescaler => 25,	-- 50MHz / (ENCTimePrescaler * 2) | (1000KHz)
		ControlerENCValBitWidth	=> 8		-- 8-bit signed encoder value
	)
	port map(
		
		Reset => BTN(0),	-- Reset button
		Clock => Clock,		-- FPGA Clock
		
		SPICLK => JC1,		-- SPI0CLK
		SPISS => JC2,		-- SPI0SS
		SPIMOSI => JC3,		-- SPI0MOSI
		SPIMISO => JC4,		-- SPI0MISO
		
		PWMOutput(0) => JB3,	-- IN1A		Motor 0 pin 0
		PWMOutput(1) => JB9,	-- IN2A		Motor 0 pin 1
		PWMEN => JB8,		-- ENA		Enable Motor 0
		
		ENCInput(0) => JA9,	-- HS1A		Motor 0 sensor 0
		ENCInput(1) => JA3	-- HS1B		Motor 0 sensor 1
	);
	
	Motor2: MotorController
	generic map(
		ControlerPWMBitWidth => 8,		-- 8-bit signed PWM
		ControlerPWMPrescaler => 25,		-- 50MHz / (PWMPrescaler * 2) | (1000KHz)
		ControlerENCTimeBitWidth => 8,		-- 8-bit timer
		ControlerENCTimePrescaler => 25,	-- 50MHz / (ENCTimePrescaler * 2) | (1000KHz)
		ControlerENCValBitWidth	=> 8		-- 8-bit signed encoder value
	)
	port map(
		
		Reset => BTN(0),	-- Reset button
		Clock => Clock,		-- FPGA Clock
		
		SPICLK => JC7,		-- SPI1CLK
		SPISS => JC8,		-- SPI1SS
		SPIMOSI => JC9,		-- SPI1MOSI
		SPIMISO => JC10,	-- SPI1MISO
		
		PWMOutput(0) => JB2,	-- IN1B		Motor 1 pin 0
		PWMOutput(1) => JB7,	-- IN2B		Motor 1 pin 1
		PWMEN => JB1,		-- ENB		Enable Motor 1
		
		ENCInput(0) => JA4,	-- HS2A		Motor 1 sensor 0
		ENCInput(1) => JA10	-- HS2B		Motor 1 sensor 1
	);
	
end behav;
