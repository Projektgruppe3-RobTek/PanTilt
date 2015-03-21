---------------------
--  ENCController  --
---------------------
library ieee;
use ieee.std_logic_1164.all;

entity ENCController is
	port(
		--
		Reset: in std_logic;
		Clock: in std_logic;
		
		-- Output
		ENCTime: out std_logic_vector(7 downto 0);
		ENCVal: out std_logic_vector(7 downto 0);
		
		-- Input
		ENCInput: in std_logic_vector(1 downto 0)
	);
end ENCController;

architecture behav of ENCController is
	
	begin
	
	ENCTime <= "00000000";
	ENCVal <= "00000000";
	
end behav;
