-------------------------
--                     --
--       Encoder       --
--                     --
-------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Encoder is
	generic(
		constant BitWidth	:	positive := 8 -- Size of the output vector
	);	
	port(
		RST	:	in  std_logic;
		CLK	:	in  std_logic;
		
		Input	:	in  std_logic_vector(1 downto 0);
		Output	:	out std_logic_vector(BitWidth-1 downto 0)
	);
end Encoder;

architecture logic of Encoder is

----------   Signals   ----------
	
	-- Contains the last input value that is different
	signal LastInput	:	std_logic_vector(1 downto 0);
	
	-- Counter value
	signal Count		:	std_logic_vector(BitWidth-1 downto 0) := (others => '0');
	
begin
	
	process(CLK)
	begin
		if rising_edge(CLK) then
			if RST = '1' then
				Count <= (others => '0');
					
			elsif Input /= LastInput then
				
				LastInput <= Input;
				
				-- Loop around positive direction - Count up
				if Input = "00" and LastInput = "11" then
					-- Prevent overflow
					if Count /= 2**(BitWidth-1)-1 then
						Count <= Count + '1';
					end if;
					
				-- Loop around negative direction - Count down
				elsif Input = "11" and LastInput = "00" then
					-- Prevent overflow
					if Count /= 2**(BitWidth-1) then
						Count <= Count - '1';
					end if;
					
				-- Count down
				elsif Input < LastInput then
					-- Prevent overflow
					if Count /= 2**(BitWidth-1) then
						Count <= Count - '1';
					end if;
					
				-- Count up
				elsif Input > LastInput then
					-- Prevent overflow
					if Count /= 2**(BitWidth-1)-1 then
						Count <= Count + '1';
					end if;
				end if;
			end if;
		end if;
		
		Output <= Count;
		
	end process;
end logic;

