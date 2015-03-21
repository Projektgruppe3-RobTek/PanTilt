-----------
-- Timer --
-----------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity Timer is
	generic(
		BitWidth: positive := 8
	);
	port(
		Reset: in std_logic;
		Clock: in std_logic;
		Output: out std_logic_vector((BitWidth - 1) downto 0)
	);
end Timer;

architecture behav of Timer is
	
	begin
	
end behav;
