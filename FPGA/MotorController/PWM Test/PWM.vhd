-------------------------
--                     --
--        PWM          --
--                     --
-------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity PWM is
	generic(
		constant BitWidth	:	positive := 8	-- Bit width of unsigned compareMatch
	);
	port(
		RST		:	in  std_logic;
		CLK		:	in  std_logic;
		
		CompareMatch	:	in  std_logic_vector(BitWidth-1 downto 0);
		Output		:	out std_logic
	);
end PWM;

architecture logic of PWM is
begin
	
	process(CLK)
		
	----------   Variables   ----------
		
		-- Direction of count
		variable CountDirection	:	boolean := true;
		
		-- Counter value
		variable Count		:	integer range 0 to 2**BitWidth-2 := 0;
		
	begin
		
		if rising_edge(CLK) then
			
			if RST = '1' then
				Count := 0;
			else
				-- Inc/Dec value
				if CountDirection then
					Count := Count + 1;
				else
					Count := Count - 1;
				end if;
				
				-- Update direction
				if Count <= 0 then
					CountDirection := true;
				elsif Count >= 2**BitWidth-2 then
					CountDirection := false;
				end if;
				
				-- Set output
				if Count >= conv_integer(unsigned(CompareMatch)) then
					Output <= '0';
				else
					Output <= '1';
				end if;
			end if;
		end if;
	end process;
end logic;
