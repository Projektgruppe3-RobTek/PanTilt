----------------------
--      Counter     --
----------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Counter is
	generic(
		size: integer := 4
	);
	port(
		input, RST: in std_logic;
		MAX: in std_logic_vector(size-1 downto 0);
		overflow_val: in std_logic_vector(size-1 downto 0) := (others => '0');
		output: out std_logic_vector(size-1 downto 0); 
		carry_out: out std_logic;
		overflow: out std_logic
	);
end Counter;

architecture logic of Counter is	 
	signal storage: std_logic_vector(size-1 downto 0);
begin

	process(input, RST) begin	
		if (RST = '1') then
			storage <= (others => '0');
		elsif (rising_edge(input)) then
			if (storage < MAX) then
				storage <= storage + '1';
				carry_out <= '0';
			else 
				storage <= (others => '0');
				carry_out <= '1';
			end if;
		end if;
	if(storage >= overflow_val ) then
		overflow <= '1';
	else
		overflow <= '0';
	end if;
	end process;	   
	
	output <= storage;
	
end logic;
