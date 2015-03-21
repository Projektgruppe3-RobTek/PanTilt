-----------
--  PWM  --
-----------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity PWM is
	generic(
		constant BitWidth: positive := 8	-- Bit width of unsigned compareMatch
	);
	port(
		-- 
		Reset: in std_logic;
		Clock: in std_logic;
		
		-- Input
		CompareMatch: in std_logic_vector((BitWidth - 1) downto 0);
		
		-- Output
		Output: out std_logic
	);
end PWM;

architecture behav of PWM is
	
	begin
	
	process (Clock)
		
		-- variables
		variable countDirection: boolean := true;
		variable value: integer := 0;
		
		begin
		
		if (rising_edge(Clock)) then
			
			-- Inc/Dec value
			if (countDirection) then
				value := value + 1;
			else
				value := value - 1;
			end if;
			
			-- Update direction
			if (value = 0) then
				countDirection := true;
			elsif (value = 2 ** BitWidth - 2) then
				countDirection := false;
			end if;
			
			-- Set output
			if (value >= to_integer(unsigned(CompareMatch))) then
				Output <= '0';
			else
				Output <= '1';
			end if;
		end if;
		
	end process;
	
end behav;
