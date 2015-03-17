-------------------
--  UnsignedPWM  --
-------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity UnsignedPWM is
	
	generic(
		-- define bit width of pwm
		constant UnsignedBitWidth: positive := 8
	);
	
	port(
		-- clock impulse to count
		CounterClock: in std_logic;
		-- compare match input vector
		CompareMatch: in std_logic_vector((UnsignedBitWidth - 1) downto 0);
		-- pwm output
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
			
			-- count value
			if (direction) then
				val := val + 1;
			else
				val := val - 1;
			end if;
			
			-- update direction
			if (val = 0) then
				direction := true;
			elsif (val = 2 ** UnsignedBitWidth - 2) then
				direction := false;
			end if;
			
			-- set output
			if (val >= to_integer(unsigned(CompareMatch))) then
				Output <= '0';
			else
				Output <= '1';
			end if;
		end if;
		
	end process;
	
end behav;
