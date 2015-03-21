---------------
-- Prescaler --
---------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity Prescaler is
	generic(
		Scale: positive := 25 -- 50MHz / (scale * 2) = 1MHz
	);
	port(
		Reset: in std_logic;
		Clock: in std_logic;
		PrescaledClock: out std_logic
	);
end Prescaler;

architecture behav of Prescaler is

	signal value: integer range 0 to Scale;
	signal output: std_logic;
	
	begin
	
	process(Clock)
		
		begin
		
		if (rising_edge(Clock)) then
			
			value <= value + 1;
			
			if (value >= Scale) then
				value <= 0;
				output <= not output;
			end if;
		end if;
		
	end process;
	
	PrescaledClock <= output;
	
end behav;

0 
1
2 --
3


0
