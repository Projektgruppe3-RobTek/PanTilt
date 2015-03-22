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
		constant ENCBitWidth	:	positive := 8;	-- Size of the input vector
		constant TimeBitWidth	:	positive := 8	-- Size of the input vector
	);
	port(
		RST	:	in  std_logic;
		
		Latch	:	in  std_logic;
		CLK	:	in  std_logic;
		
		PIENC	:	in  std_logic_vector(ENCBitWidth-1 downto 0);
		PITime	:	in  std_logic_vector(TimeBitWidth-1 downto 0);
		
		SO	:	out std_logic
	);
end PISO;

architecture logic of PISO is
	
----------   Signals   ----------
	
	-- Data in shift register
	signal Data: std_logic_vector(ENCBitWidth+TimeBitWidth-1 downto 0);
	
begin
	
	process(CLK, Latch, RST) begin
		
		-- Reset shift register
		if RST = '1' then
			Data <= (others => '0');
			
		-- Latch data into shift register
		elsif LATCH = '1' then
			Data(ENCBitWidth+TimeBitWidth-1 downto TimeBitWidth) <= PIENC;
			Data(TimeBitWidth-1 downto 0) <= PITime;
			SO <= Data(ENCBitWidth+TimeBitWidth-1);
			
		-- Clock data out of shift register
		elsif falling_edge(CLK) then
			Data(ENCBitWidth+TimeBitWidth-1 downto 1) <= Data(ENCBitWidth+TimeBitWidth-2 downto 0);
			Data(0) <= '0';
			SO <= Data(ENCBitWidth+TimeBitWidth-1);
		end if;
	end process;
end logic;
