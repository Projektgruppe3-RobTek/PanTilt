----------------------
--  DisplayDecoder  --
----------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity DisplayDecoder is
	port(
		I: in std_logic_vector(3 downto 0);
		O: out std_logic_vector(7 downto 0)
	);
end DisplayDecoder;

architecture behav of DisplayDecoder is
	
	signal s: std_logic_vector(15 downto 0);
	
	begin
	
	s(0) <= not I(0) and not I(1) and not I(2) and not I(3);
	s(1) <= I(0) and not I(1) and not I(2) and not I(3);
	s(2) <= not I(0) and I(1) and not I(2) and not I(3);
	s(3) <= I(0) and I(1) and not I(2) and not I(3);
	s(4) <= not I(0) and not I(1) and I(2) and not I(3);
	s(5) <= I(0) and not I(1) and I(2) and not I(3);
	s(6) <= not I(0) and I(1) and I(2) and not I(3);
	s(7) <= I(0) and I(1) and I(2) and not I(3);
	s(8) <= not I(0) and not I(1) and not I(2) and I(3);
	s(9) <= I(0) and not I(1) and not I(2) and I(3);
	s(10) <= not I(0) and I(1) and not I(2) and I(3);
	s(11) <= I(0) and I(1) and not I(2) and I(3);
	s(12) <= not I(0) and not I(1) and I(2) and I(3);
	s(13) <= I(0) and not I(1) and I(2) and I(3);
	s(14) <= not I(0) and I(1) and I(2) and I(3);
	s(15) <= I(0) and I(1) and I(2) and I(3);
	
	O(0) <= not (s(0) or s(2) or s(3) or s(5) or s(6) or s(7) or s(8) or s(9) or s(10) or s(12) or s(14) or s(15));
	O(1) <= not( s(0) or s(1) or s(2) or s(3) or s(4) or s(7) or s(8) or s(9) or s(10) or s(13));
	O(2) <= not (s(0) or s(1) or s(3) or s(4) or s(5) or s(6) or s(7) or s(8) or s(9) or s(10) or s(11) or s(13));
	O(3) <= not (s(0) or s(2) or s(3) or s(5) or s(6) or s(8) or s(9) or s(11) or s(12) or s(13) or s(14));
	O(4) <= not (s(0) or s(2) or s(6) or s(8) or s(10) or s(11) or s(12) or s(13) or s(14) or s(15));
	O(5) <= not (s(0) or s(4) or s(5) or s(6) or s(8) or s(9) or s(10) or s(11) or s(12) or s(14) or s(15));
	O(6) <= not (s(2) or s(3) or s(4) or s(5) or s(6) or s(8) or s(9) or s(10) or s(11) or s(13) or s(14) or s(15));
	O(7) <= '1';
	
end behav;
