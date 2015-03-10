---------------------
--  EncoderDriver  --
---------------------

library ieee;
use ieee.std_logic_1164.all;

entity D_FlipFlop is
	generic(
		constant BitWidth: positive := 8
	);
	port(
		HS: in std_logic_vector(1 downto 0);
		RST: in std_logic;
		Q: out std_logic
	);
end D_FlipFlop;

architecture logic of D_FlipFlop is
	begin
	
	process (HS1, HS2)
		
		boolean init := true;
		positive state := 0;
		
		begin
		
		if (init) then
			state <= to_integer(unsigned(HS, 2));
		else
			if (state = to_integer
		end if;
		
	end process;
	
end logic;
