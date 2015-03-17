----------------------
--    T_FlipFlop    --
----------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity T_FlipFlop is
	port(
		input		: 	in std_logic;
		output	: 	out std_logic
	);
end T_FlipFlop;

architecture logic of T_FlipFlop is	 
	signal storage: std_logic;
begin
   
	process(input) begin
		if (input'event and input = '1') then 		
			storage <= not storage;
		end if;
	end process;	   
	
	output <= storage;
	
end logic;