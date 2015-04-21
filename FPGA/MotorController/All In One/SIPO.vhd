-------------------------
--                     --
--        SIPO         --
--                     --
-------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity SIPO is
	generic(
		constant BitWidth	:	positive := 10	-- Size of the output vector
	);
	port(
		RST		:	in  std_logic;
		
		LATCH	: 	in  std_logic;
		CLK		: 	in  std_logic;
		
		RW		: 	out std_logic;
		SPIRST	:	out std_logic;
		
		SI		: 	in  std_logic;
		SO		:	out std_logic;
		PO		:	out std_logic_vector(BitWidth-3 downto 0)
	);
end SIPO;

architecture logic of SIPO is
	
----------   Signals   ----------
	
	-- Data in shift register
	signal Data	:	std_logic_vector(BitWidth-1 downto 0);
	
	-- Data in output register
	signal Output	:	std_logic_vector(BitWidth-3 downto 0);
	
begin
	
	process(CLK, Latch, RST) begin
		
		-- Reset shift register
		if RST = '1' then
			Data <= (others => '0');
			Output <= (others => '0');
			RW <= '0';
			
		-- Latch data to output
		elsif Latch = '1' then
			RW <= Data(BitWidth-2);
			
			-- Latch if ReadWrite-bit = '0'
			if Data(BitWidth-2) = '0' then
				SPIRST <= Data(BitWidth-1);
				Output <= Data(BitWidth-3 downto 0);
			end if;
			
		-- Clock data into shift register
		elsif rising_edge(CLK) then
			SO <= Data(BitWidth-1);
			Data(BitWidth-1 downto 1) <= Data(BitWidth-2 downto 0);
			Data(0) <= SI;
		end if;
		
		PO <= Output;
		
	end process;
end logic;
