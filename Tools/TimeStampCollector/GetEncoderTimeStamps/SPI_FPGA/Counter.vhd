library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity COUNTER_X_BIT is
	generic (
		SIZE:	integer range 0 to 100000 --Size of counter in bits
					);
	port(
	D, RST: IN STD_LOGIC;
	Q: OUT STD_LOGIC_VECTOR(SIZE-1 downto 0);
	MAX: IN STD_LOGIC_VECTOR(SIZE-1 downto 0)
			);
end COUNTER_X_BIT;

architecture behav  of COUNTER_X_BIT is
signal tmp_Q: STD_LOGIC_VECTOR(SIZE-1 downto 0);

begin

Q <= tmp_Q;
process (D, RST) 
begin
   	if RST='1' then 
    	tmp_Q <= (others => '0');
   	elsif D='1' and D'event then
   		if tmp_Q < MAX then
   			tmp_Q <= tmp_Q + '1';
   		else
   			tmp_Q <= (others => '0');
   		end if;
   	end if;
end process;

	
end behav;
