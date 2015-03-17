------------------
--  D_FlipFlop  --
------------------
library ieee;
use ieee.std_logic_1164.all;

entity D_FlipFlop is
	port(
		D: in std_logic;
		CLK: in std_logic;
		RST: in std_logic;
		Q: out std_logic
	);
end D_FlipFlop;

architecture logic of D_FlipFlop is
	begin
	
	process (CLK, RST)
		begin
		
		if (RST = '0') then
			Q <= '0';
		elsif (rising_edge(CLK)) then
			Q <= D;
		end if;
		
	end process;
	
end logic;
