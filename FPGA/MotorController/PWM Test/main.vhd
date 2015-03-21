------------
--  main  --
------------
library ieee;
use ieee.std_logic_1164.all;

entity main is
	port(
		-- 
		Clock: in std_logic;			-- FPGA Clock
		
		-- Input
		SW: in std_logic_vector(7 downto 0);	-- CompareMatch Input
		
		-- Motor0
		JB8: out std_logic;	-- ENA		Enable Motor 0
		JB3: out std_logic;	-- IN1A		Motor 0 pin 0
		JB9: out std_logic	-- IN2A		Motor 0 pin 1
	);
end main;

architecture behav of main is
	
	component PWMController is
		generic(
			-- PWM Prescaler value
			constant PWMClockPrescaler: positive := 5;	-- Prescaled Clock = 50MHz / (PWMClockPrescaler * 2)
									-- PWMClockPrescaler -> 50MHz / (5 * 2) = 5MHz;
			-- PWM compare match bit width
			constant PWMBitWidth: positive := 8		-- PWM Frequency = 50MHz / (PWMClockPrescaler * 2 * (2 ** PWMBitWidth) - 4)
									-- PWM Frequency -> 50MHz / (5 * 2 * (2 ** 8) - 4) = 20KHz
		);
		port(
			-- 
			Reset: in std_logic;
			Clock: in std_logic;
		
			-- Input
			PWMCompareMatch: in std_logic_vector((PWMBitWidth - 1) downto 0);
		
			-- Output
			PWMOutput: out std_logic_vector(1 downto 0)
		);
	end component;
	
	signal sClock: std_logic;
	
	begin
	
	sClock <= Clock;
	
	JB8 <= '1';
	
	PWMController0: PWMController
	generic map(
		PWMClockPrescaler => 5,	-- Prescaled Clock = 50MHz / (PWMClockPrescaler * 2)
					-- PWMClockPrescaler -> 50MHz / (5 * 2) = 5MHz;
		PWMBitWidth => 8	-- PWM Frequency = 50MHz / (PWMClockPrescaler * 2 * (2 ** PWMBitWidth) - 4)
					-- PWM Frequency -> 50MHz / (5 * 2 * (2 ** 8) - 4) = 20KHz
	)
	port map(
		Reset => '0',
		Clock => sClock,
		PWMCompareMatch => SW(7 downto 0),
		PWMOutput(0) => JB3,
		PWMOutput(1) => JB9
	);
	
end behav;
