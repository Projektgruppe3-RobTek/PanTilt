---------------
-- Prescaler --
---------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity Prescaler is
	generic(
		Scale: positive := 50000 -- 50MHz / scale = 1000Hz
	);
	port(
		Reset: in std_logic;
		Clock: in std_logic;
		PrescaledClock: out std_logic
	);
end Prescaler;

architecture behav of Prescaler is

	signal value: integer range 0 to Scale / 2;
	signal output: std_logic;
	
	begin
	
	process(Clock)
		
		begin
		
		if (Reset = '1') then
			value <= 0;
			output <= '0';
		elsif rising_edge(Clock) then
			if value = Scale / 2 then
				value <= 0;
				output <= not output;
			else
				value <= value + 1;
			end if;
		end if;
		
	end process;
	
	PrescaledClock <= output;
	
end behav;
