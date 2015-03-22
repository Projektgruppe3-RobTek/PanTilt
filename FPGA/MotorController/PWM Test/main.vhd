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
		BTN	:	in  std_logic_vector(0 downto 0);	-- Reset
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
			constant CLKScale	:	positive := 5;	-- Prescaled Clock = 50MHz / (PWMClockPrescaler * 2)
									-- PWMClockPrescaler -> 50MHz / (5 * 2) = 5MHz;
			-- PWM compare match bit width
			constant BitWidth	:	positive := 8	-- PWM Frequency = 50MHz / (PWMClockPrescaler * 2 * (2 ** PWMBitWidth) - 4)
									-- PWM Frequency -> 50MHz / (5 * 2 * (2 ** 8) - 4) = 20KHz
		);
		port(
			RST		:	in  std_logic;
			CLK		:	in  std_logic;
		
			PWMCM		:	in  std_logic_vector((BitWidth - 1) downto 0);
			
			PWMOutput	:	out std_logic_vector(1 downto 0)
		);
	end component;
	
begin
	
	-- Enable H-Bridge
	JB8 <= '1';
	
	PWMController0: PWMController
	generic map(
		CLKScale => 5,	-- Prescaled Clock = 50MHz / (PWMClockPrescaler * 2)
				-- PWMClockPrescaler -> 50MHz / (5 * 2) = 5MHz;
		BitWidth => 8	-- PWM Frequency = 50MHz / (PWMClockPrescaler * 2 * (2 ** PWMBitWidth) - 4)
				-- PWM Frequency -> 50MHz / (5 * 2 * (2 ** 8) - 4) = 20KHz
	)
	port map(
		RST => BTN(0),
		CLK => CLK,
		
		PWMCM => SW,
		
		PWMOutput(0) => JB3,
		PWMOutput(1) => JB9
	);
	
end logic;
