-------------------------
--                     --
--       Pulser        --
--                     --
-------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Pulser is
		port(
			RST		:	in  std_logic;
			CLK		: 	in  std_logic;
			
			Input		:	in  std_logic;
			Output		:	out std_logic
		);
end Pulser;

architecture logic of Pulser is
	
----------   Signals   ----------
	
	-- Data in shift register
	signal InputDelay	:	std_logic;
	
begin
	
	process(CLK) begin
		if rising_edge(CLK) then
		
			-- Reset
			if RST = '1' then
				InputDelay <= '0';			
				Output <= '0';
			
			else
				InputDelay <= Input;
				
				-- If there is a rising edge pulse output
				if InputDelay = '0' and Input = '1' then
					Output <= '1';
				else
					Output <= '0';
				end if;
			end if;
		end if;
	end process;
end logic;
