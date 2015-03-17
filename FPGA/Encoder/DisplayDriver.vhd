---------------------
--  DisplayDriver  --
---------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity DisplayDriver is
	port(
		I0, I1: in std_logic_vector(7 downto 0);
		C: in std_logic;
		DP_Output: out std_logic_vector(7 downto 0);
		AN_Output: out std_logic_vector(3 downto 0)
	);
end DisplayDriver;

architecture behav of DisplayDriver is
	
	component DisplayDecoder is
		port(
			I: in std_logic_vector(3 downto 0);
			O: out std_logic_vector(7 downto 0)
		);
	end component;
	
	component T_FlipFlop is
		port(
			T, S, R: in std_logic;
			Q, IQ: out std_logic
		);
	end component;
	
	component Prescaler is
		port(
			I, R: in std_logic;
			S: in std_logic_vector(3 downto 0);
			O: out std_logic
		);
	end component;
	
	signal state, Istate: std_logic_vector(1 downto 0);
	signal res: std_logic_vector(3 downto 0);
	signal PC: std_logic;
	
	begin
	
	AN_Output <= 
		"1110" when state = "00" else
		"1101" when state = "01" else
		"1011" when state = "10" else
		"0111" when state = "11";
	
	res <=
		I0(3 downto 0) when state = "00" else
		I0(7 downto 4) when state = "01" else
		I1(3 downto 0) when state = "10" else
		I1(7 downto 4) when state = "11";
	
	prescaler0: Prescaler port map(I => C, R => '0', S => "1111", O => PC);
	
	flipFlop0: T_FlipFlop port map(T => PC, S => '0', R => '0', Q => state(0), IQ => Istate(0));
	flipFlop1: T_FlipFlop port map(T => Istate(0), S => '0', R => '0', Q => state(1), IQ => Istate(1));
	
	display: DisplayDecoder port map(I => res, O => DP_Output);
	
end behav;
