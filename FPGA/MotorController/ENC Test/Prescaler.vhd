---------------------------
--                       --
--       Prescaler       --
--                       --
---------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Prescaler is
	generic(
		constant Scale	:	positive := 25 -- 50MHz / (Scale * 2) = 1MHz
	);
	port(
		RST		:	in  std_logic;
		CLK		:	in  std_logic;
		
		ScaledCLK	:	out std_logic
	);
end Prescaler;


architecture logic of Prescaler is
	
----------   Signals   ----------
	
	-- Contains the scaled clock
	signal Output	:	std_logic;
	
	-- Counter value
	signal Count	:	integer range 0 to Scale;
	
begin
	
	process(CLK)
	begin
		if RST = '1' then
			Count <= 0;
			Output <= '0';
			
		elsif rising_edge(CLK) then
			
			Count <= Count + 1;
			
			if Count >= Scale then
				Count <= 0;
				Output <= not Output;
			end if;
		end if;
	end process;
	
	ScaledCLK <= Output;
	
end logic;
