library ieee;
use ieee.std_logic_1164.all;

entity MEMORY_X_BIT is
	generic (
		SIZE:	integer range 0 to 100000 --Size of counter in bits
					);  
  port (
    in_val   : in std_logic_vector(SIZE-1 downto 0);
    reset : in std_logic;
    out_val  : out std_logic_vector(SIZE-1 downto 0);
    write    : in std_logic;
    clk: in std_logic
    );

end MEMORY_X_BIT;

architecture behav of MEMORY_X_BIT is
signal prevwrite : std_logic := '0';
begin
process (clk)
begin  -- process
	if clk'event and clk='1' then
		if (reset = '1') then
		  out_val <= (others => '0');
		elsif write /= prevwrite then
		  out_val <= in_val;
		end if;
		prevwrite <= write;
  end if;
end process;
  

end behav;
