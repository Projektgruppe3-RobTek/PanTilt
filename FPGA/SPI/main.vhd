------------
--  main  --
------------
library ieee;
use ieee.std_logic_1164.all;

entity main is
	port(
		BTN: in std_logic_vector(2 downto 0);
		SW: in std_logic_vector(7 downto 0);
		LD: out std_logic_vector(7 downto 0);
		Clock: in std_logic
	);
end main;

architecture behav of main is
	
	component PISO is
		generic(
			constant Data_Width: positive := 8
		);
		port(
			CLK: in std_logic;
			RST: in std_logic;
			LATCH: in std_logic;
			Input: in std_logic_vector((8 - 1) downto 0);
			Debug: out std_logic_vector((8 - 1) downto 0);
			Output: out std_logic
		);
	end component;
	
	component Debounce is
		generic(
			counter_size: integer := 19
		); 
		port(
			clk_in, input: in std_logic;
			output: out std_logic
		);
	end component;
	
	signal BTN0, BTN1, BTN2: std_logic;
	
	begin
	
	Debounce0: Debounce port map(clk_in => Clock, Input => BTN(0), Output => BTN0);
	Debounce1: Debounce port map(clk_in => Clock, Input => BTN(1), Output => BTN1);
	Debounce2: Debounce port map(clk_in => Clock, Input => BTN(2), Output => BTN2);
	
	PISO0: PISO 
		generic map(
			Data_Width => 8
		)
		port map(
			CLK => BTN0,
			RST => not BTN2,
			LATCH => BTN1,
			Input => SW,
			Output => LD(0)
		)
	;
end behav;
