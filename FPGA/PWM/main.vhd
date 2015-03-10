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
		
		JA1: in std_logic;
		JA2: in std_logic;
		JA3: in std_logic;
		JA5: in std_logic;
		JA6: in std_logic;
		JA7: in std_logic;
		
		JB0: out std_logic;
		JB1: out std_logic;
		JB2: out std_logic;
		JB4: out std_logic;
		JB5: out std_logic;
		JB6: out std_logic;
		
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
	
	component Memory1Byte is
		port(
			EI, R, EO: in std_logic;
			I: in std_logic_vector(7 downto 0);
			O: out std_logic_vector(7 downto 0)
		);
	end component;
	
	signal Motor1, Motor2, sEI: std_logic_vector(1 downto 0);
	signal Speed1, Speed2: std_logic_vector(7 downto 0);
	
	begin
	
	sEI(0) <= BTN(0) and (not BTN(3));
	sEI(1) <= BTN(1) and (not BTN(3));
	
	LD(0) <= JA6;
	LD(1) <= JA2;
	LD(2) <= JA3;
	LD(3) <= JA7;
	LD(4) <= JA1;
	LD(5) <= JA5;
	LD(6) <= '0';
	LD(7) <= '0';
	
	JB5 <= '1';
	JB0 <= '1';
	
	JB3 <= Motor1(0);
	JB9 <= Motor1(1);
	JB2 <= Motor2(0);
	JB7 <= Motor2(1);
	
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
	
	Mem1: Memory1Byte port map(
		EI => sEI(0),
		R => BTN(2),
		EO => '1',
		I => SW,
		O => Speed1
	);
	
	Mem2: Memory1Byte port map(
		EI => sEI(1),
		R => BTN(2),
		EO => '1',
		I => SW,
		O => Speed2
	);
	
end behav;
