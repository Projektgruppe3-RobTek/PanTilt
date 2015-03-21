---------------------------
--                       --
--         main          --
--                       --
---------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity main is
	port(
		CLK	:	in  std_logic;			-- FPGA Clock
		
		SW	:	in  std_logic_vector(7 downto 0);	-- CompareMatch Input
		
		-- Motor0
		JB8	:	out std_logic;	-- ENA		Enable Motor 0
		JB3	:	out std_logic;	-- IN1A		Motor 0 pin 0
		JB9	:	out std_logic	-- IN2A		Motor 0 pin 1
	);
end main;

architecture logic of main is
	
----------   Components   ----------
	
	component PWMController is
		generic(
			-- PWM Prescaler value
			constant PWMCLKScale	:	positive := 5;	-- Prescaled Clock = 50MHz / (PWMClockPrescaler * 2)
									-- PWMClockPrescaler -> 50MHz / (5 * 2) = 5MHz;
			-- PWM compare match bit width
			constant PWMBitWidth	:	positive := 8	-- PWM Frequency = 50MHz / (PWMClockPrescaler * 2 * (2 ** PWMBitWidth) - 4)
									-- PWM Frequency -> 50MHz / (5 * 2 * (2 ** 8) - 4) = 20KHz
		);
		port(
			RST		:	in  std_logic;
			CLK		:	in  std_logic;
		
			PWMCompareMatch	:	in  std_logic_vector((PWMBitWidth - 1) downto 0);
			PWMOutput	:	out std_logic_vector(1 downto 0)
		);
	end component;
	
begin
	
	-- Enable H-Bridge
	JB8 <= '1';
	
	PWMController0: PWMController
	generic map(
		PWMCLKScale => 5,	-- Prescaled Clock = 50MHz / (PWMClockPrescaler * 2)
					-- PWMClockPrescaler -> 50MHz / (5 * 2) = 5MHz;
		PWMBitWidth => 8	-- PWM Frequency = 50MHz / (PWMClockPrescaler * 2 * (2 ** PWMBitWidth) - 4)
					-- PWM Frequency -> 50MHz / (5 * 2 * (2 ** 8) - 4) = 20KHz
	)
	port map(
		RST => '0',
		CLK => CLK,
		PWMCompareMatch => SW(7 downto 0),
		PWMOutput(0) => JB3,
		PWMOutput(1) => JB9
	);
	
end logic;
