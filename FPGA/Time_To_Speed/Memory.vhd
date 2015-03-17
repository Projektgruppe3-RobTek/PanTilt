----------------------
--      Memory      --
----------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Memory is
	port(
		input: in std_logic_vector(15 downto 0);
		SET, RST, clk_in: in std_logic;
		output: out std_logic_vector(15 downto 0)
	);
end Memory;

architecture logic of Memory is	 
begin
   
	process(clk_in) begin
		if clk_in'event and clk_in = '1' then
			if SET = '1' then 		
				output <= input;
			elsif (RST = '1') then
				output <= (others => '0');
			end if;
		end if;
	end process;	   
end logic;