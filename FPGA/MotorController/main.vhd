------------
--  main  --
------------
library ieee;
use ieee.std_logic_1164.all;

entity main is
	port(
		BTN: in std_logic_vector(0 downto 0); -- Reset
		LD: in std_logic_vector(5 downto 0);
		
		Clock: in std_logic; -- FPGA Clock
		
		JA2: in std_logic;  -- Index0
		JA3: in std_logic;  -- HS1B
		JA4: in std_logic;  -- HS2A
		JA8: in std_logic;  -- Index1
		JA9: in std_logic;  -- HS1A
		JA10: in std_logic; -- HS2B
		
		JB1: out std_logic;  -- ENB
		JB2: out std_logic;  -- IN1B
		JB3: out std_logic;  -- IN1A
		JB7: out std_logic;  -- IN2B
		JB8: out std_logic;  -- ENA
		JB9: out std_logic;  -- IN2A
		
		JC1: out std_logic;  -- CLK
		JC2: out std_logic;  -- !SS
		JC3: out std_logic;  -- MOSI
		JC4: out std_logic;  -- MISO
		JC7: out std_logic   -- SS
	);
end main;

architecture behav of main is
	
	component MotorController is
		port(
			CLK: in std_logic;
			
			SS: in std_logic;
			
			SPICLK: in std_logic;
			SPISS: in std_logic;
			SPIMOSI: in std_logic;
			SPIMISO: out std_logic;
			
			Reset: in std_logic;
			
			IN1: out std_logic;
			IN2: out std_logic;
			
			HSA: in std_logic;
			HSB: in std_logic
		);
	end component;
	
	signaL SPISS1, SPISS2, SPIMISO1, SPIMISO2 std_logic;
	
	begin
	
	LD(0) <= JA2; -- Index0
	LD(1) <= JA8; -- Index1
	LD(2) <= JA2; -- HS1A
	LD(3) <= JA8; -- HS1B
	LD(4) <= JA2; -- HS2A
	LD(5) <= JA8; -- HS2B
	
	SPIMISO <= SPIMISO1 or SPIMISO2;
	
	SPISS1 <= SPISS when SS = '1' else '0';
	SPISS2 <= SPISS when SS = '0' else '0';
	
	Motor1: MotorController port map(
		CLK => Clock,
		SPICLK => JC1,
		SPISS => SPISS1,
		SPIMOSI => JC3,
		SPIMISO => SPIMISO1,
		Reset => BTN(0),
		IN1 => JB3, -- IN1A
		IN2 => JB9, -- IN2A
		HSA => JA9, -- HS1A
		HSB => JA3  -- HS1B
	);
	
	Motor2: MotorController port map(
		CLK => Clock,
		SPICLK => JC1,
		SPISS => SPISS2,
		SPIMOSI => JC3,
		SPIMISO => SPIMISO2,
		Reset => BTN(0),
		IN1 => JB2, -- IN1B
		IN2 => JB7, -- IN2B
		HSA => JA4, -- HS2A
		HSB => JA10 -- HS2B
	);
	
end behav;
