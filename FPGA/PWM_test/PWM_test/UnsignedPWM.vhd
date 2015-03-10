-------------------
--  UnsignedPWM  --
-------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_arith.all;

entity UnsignedPWM is
	
	generic(
		constant UnsignedBitWidth: positive := 7
	);
	
	port(
		CounterClock: in std_logic;
		CompareMatch: in std_logic_vector((UnsignedBitWidth - 1) downto 0);
		Output: out std_logic
	);
	
end UnsignedPWM;

architecture behav of UnsignedPWM is
	
	begin
	
	process (CounterClock)
		
		variable direction: boolean := true;
		variable val: integer := 0;
		
		begin
		
		if (rising_edge(CounterClock)) then
		
			if (direction) then
				val := val + 1;
			else
				val := val - 1;
			end if;
			
			if (val = 0) then
				direction := true;
			elsif (val = 2 ** UnsignedBitWidth - 1) then
				direction := false;
			end if;
			
			
			if (val >= to_integer(unsigned(CompareMatch))) then
				Output <= '0';
			else
				Output <= '1';
			end if;
		end if;
		
	end process;
	
end behav;
