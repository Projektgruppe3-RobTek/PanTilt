-------------------------
--                     --
--        PISO         --
--                     --
-------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity PISO is
	generic(
		constant BitWidth	:	positive := 8	-- Size of the input vector
	);
	port(
		RST	:	in  std_logic;
		
		LATCH	:	in  std_logic;
		CLK	:	in  std_logic;
		
		SI	:	in  std_logic := '0';
		PI	:	in  std_logic_vector(BitWidth-1 downto 0);
		SO	:	out std_logic
	);
end PISO;

architecture logic of PISO is
	
----------   Signals   ----------
	
	-- Data in shift register
	signal Data: std_logic_vector(BitWidth-1 downto 0);
	
begin
	
	process(CLK, LATCH, RST) begin
		
		-- Reset shift register
		if RST = '1' then
			Data <= (others => '0');
			
		-- Latch data into shift register
		elsif LATCH = '1' then
			Data <= PI;
			SO <= Data(BitWidth-1);
			
		-- Clock data out of shift register
		elsif falling_edge(CLK) then
			SO <= Data(BitWidth-1);
			Data(BitWidth-1 downto 1) <= Data(BitWidth-2 downto 0);
			Data(0) <= SI;
		end if;
	end process;
end logic;
