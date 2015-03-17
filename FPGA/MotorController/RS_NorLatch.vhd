-------------------
--  RS_NorLatch  --
-------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity RS_NorLatch is
	port(
		S, R: in std_logic;
		Q: out std_logic
	);
end RS_NorLatch;

architecture behav of RS_NorLatch is
	
	signal s0, s1: std_logic;
	
	begin
	
	s0 <= S nor s1;
	s1 <= R nor s0;
	
	Q <= s1;
	
end behav;
