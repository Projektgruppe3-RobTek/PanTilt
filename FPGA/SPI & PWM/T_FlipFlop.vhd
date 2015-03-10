------------------
--  T_FlipFlop  --
------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity T_FlipFlop is
	port(
		T, S, R: in std_logic;
		Q, IQ: out std_logic
	);
end T_FlipFlop;
 
architecture behav of T_FlipFlop is	 
	
	signal temp: std_logic;
	
	begin
   
	process (T, R, S) 
		begin
		if R = '1' then   
			temp <= '0'; 
		elsif S = '1' then
			temp <= '1';
		elsif (rising_edge(T)) then 		
			temp <= '1' xor temp;
		end if;
	end process;	   
	
	Q <= temp;
	IQ <= not temp;
	
end behav;
