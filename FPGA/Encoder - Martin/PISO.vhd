library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity PISO is
	port(
		ID: in std_logic;
		SI: in std_logic;
		CLK: in std_logic;
		RST: in std_logic;
		LATCH: in std_logic;
		PI: in std_logic_vector(7 downto 0);
		test: out std_logic_vector(7 downto 0);
		SO: out std_logic
	);
end PISO;

architecture logic of PISO is
	signal storage: std_logic_vector(8 downto 0);
begin

	process (CLK) begin
		if rising_edge(CLK) then
			if (RST = '1') then
				storage(7 downto 0) <= (others => '0');
				storage(8) <= ID;
			elsif (LATCH = '1') then
				storage(7 downto 0) <= PI;
			elsif (LATCH = '0') then
				storage(8 downto 1) <= storage(7 downto 0);
				storage(0) <= SI;
				SO <= storage(8);
			end if;
		end if;
	end process;
	
	test <= storage(7 downto 0);
end logic;

