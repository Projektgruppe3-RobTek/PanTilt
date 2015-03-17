-----------------------
--  Display_Driver   --
-----------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Display_Driver is
	port(
		input: in std_logic_vector(3 downto 0);
		output: out std_logic_vector(6 downto 0)
	);
end Display_Driver;

architecture logic of Display_Driver is	 
begin
   
	process(input) begin
		case input is
			when "0001" => output <= "1111001";	--1
			when "0010" => output <= "0100100";	--2
			when "0011" => output <= "0110000";	--3
			when "0100" => output <= "0011001";	--4
			when "0101" => output <= "0010010";	--5
			when "0110" => output <= "0000010";	--6
			when "0111" => output <= "1111000";	--7
			when "1000" => output <= "0000000";	--8
			when "1001" => output <= "0010000";	--9
			when "1010" => output <= "0001000";	--A
			when "1011" => output <= "0000011";	--b
			when "1100" => output <= "1000110";	--C
			when "1101" => output <= "0100001";	--d
			when "1110" => output <= "0000110";	--E
			when "1111" => output <= "0001110";	--F
			when others => output <= "1000000";	--0
		end case;
	end process;	   
end logic;