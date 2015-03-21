----------
-- PISO --
----------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity PISO is
	generic(
		constant PISOBitWidth: positive := 8
	);
	port(
		Reset: in std_logic;
		Clock: in std_logic;
		
		Latch: in std_logic;
		
		SerialInput: in std_logic;
		SerialOutput: out std_logic;
		
		ParallelInput: in std_logic_vector((PISOBitWidth - 1) downto 0)
	);
end PISO;

architecture behav of PISO is

	begin
	
	SerialOutput <= '0';
	
end behav;
