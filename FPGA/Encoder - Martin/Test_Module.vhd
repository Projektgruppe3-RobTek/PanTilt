library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.all;

entity Test_Module is
	generic(
		data_width	:	integer 	 := 8
	);
	port(
		clk	:	in  std_logic; -- SPI clock
		ss_n	: 	in  std_logic; -- Slave select, active low
		output:	out std_logic_vector(data_width-1 downto 0)
	);
end Test_Module;

architecture logic of Test_Module is
	signal bit_count		: 	std_logic_vector(data_width downto 0); -- Transaction bits -> '1' = active
begin

	-- Keep track of which bit is active
	process(clk, ss_n) begin
		if(ss_n = '1') then -- If slave not selected or being reset
			bit_count(data_width) <= '1';
			bit_count(data_width-1 downto 0) <= (others => '0'); -- Reset bit_count
		else
			if(rising_edge(clk)) then -- Slave selected
				bit_count(data_width-1 downto 0) <= bit_count(data_width downto 1); -- Shift register that keep track of which bit is active
				bit_count(data_width) <= ss_n;
			end if;
		end if;
	end process;
	
	output <= bit_count(7 downto 0);
	
end logic;

