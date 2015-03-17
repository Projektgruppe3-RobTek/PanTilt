------------
--  main  --
------------
library ieee;
use ieee.std_logic_1164.all;

entity main is
	port(
		BTN: in std_logic_vector(3 downto 0);
		Clock: in std_logic;
		SW: in std_logic_vector(7 downto 0);
		
		JB1: out std_logic;
		JB2: out std_logic;
		JB3: out std_logic;
		JB7: out std_logic;
		JB8: out std_logic;
		JB9: out std_logic;
		
		LD: out std_logic_vector(7 downto 0)
	);
end main;

architecture behav of main is
	
	component SignedPWM is
	
		generic(
			constant SignedBitWidth: positive := 8
		);
	
		port(
			CounterClock: in std_logic;
			CompareMatch: in std_logic_vector((SignedBitWidth - 1) downto 0);
			Output: out std_logic_vector(1 downto 0)
		);
	
	end component;
	
	component Memory is
		generic(
			BitWidth: positive
		);
		port(
			EI, R, EO: in std_logic;
			Input: in std_logic_vector(7 downto 0);
			O: out std_logic_vector(7 downto 0)
		);
	end component;
	
	signal Motor1, Motor2: std_logic_vector(1 downto 0);
	signal Speed1, Speed2: std_logic_vector(7 downto 0);
	
	begin
	
	-- enable h-bridge
	JB8 <= '1'; -- ENA
	JB1 <= '1'; -- ENB
	
	-- set motor output
	JB3 <= Motor1(0); -- IN1A
	JB9 <= Motor1(1); -- IN2A
	JB2 <= Motor2(0); -- IN1B
	JB7 <= Motor2(1); -- IN2B
	
	LD <= SW;
	
	PWM1: SignedPWM
		generic map(
			SignedBitWidth => 8
		)
		port map(
			CounterClock => Clock,
			CompareMatch => speed1,
			Output => Motor1
		)
	;
	
	PWM2: SignedPWM
		generic map(
			SignedBitWidth => 8
		)
		port map(
			CounterClock => Clock,
			CompareMatch => speed2,
			Output => Motor2
		)
	;
	
	Mem1: Memory
		generic map(
			BitWidth => 8
		)
		port map(
			EI => BTN(1),
			R => BTN(2),
			EO => '1',
			Input => SW,
			O => Speed1
		)
	;
	
	Mem2: Memory 
		generic map(
			BitWidth => 8
		)
		port map(
			EI => BTN(0),
			R => BTN(2),
			EO => '1',
			Input => SW,
			O => Speed2
		)
	;
	
end behav;
