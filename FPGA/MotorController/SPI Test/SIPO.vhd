----------
-- SIPO --
----------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity SIPO is
	generic(
		constant PWMBitWidth: positive := 8
	);
	port(
		Reset: in std_logic;
		Clock: in std_logic;
		
		Latch: in std_logic;
		
		SerialInput: in std_logic;
		
		ReadWrite: out std_logic;
		
		ParallelOutput: out std_logic_vector((BitWidth - 1) downto 0)
	);
end SIPO;

architecture behav of SIPO is
	
	begin
	
	ParallelOutput <= (others => '0');
	ReadWrite <= '0';
	
end behav;
