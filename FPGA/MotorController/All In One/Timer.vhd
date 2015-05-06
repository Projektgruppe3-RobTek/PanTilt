---------------------------
--                       --
--        Counter        --
--                       --
---------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Timer is
	generic(
		constant BitWidth	:	positive := 8 -- Bit width of counter
	);
	port(
		RST	:	in std_logic;
		CLK	:	in std_logic;
		
		Output	:	out std_logic_vector(BitWidth-1 downto 0)
	);
end Counter;

architecture logic of Counter is
	
----------   Signals   ----------
	
	-- Counter value
	signal Count	:	std_logic_vector(BitWidth-1 downto 0) := (others => '0');
	
begin
	
	process(CLK)
	begin	
		if RST = '1' then
			Count <= (others => '0');
			
		elsif rising_edge(CLK) then
			if Count < 2**BitWidth-1 then
				Count <= Count + '1';
			end if;
		end if;
	end process;	   
	
	Output <= Count;
	
end logic;
