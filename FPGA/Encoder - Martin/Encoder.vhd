library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity Encoder is
	port(
		RST		: 	in  std_logic;
		Input		:	in  std_logic_vector(1 downto 0);
		Test		:	out std_logic_vector(1 downto 0);
		Output	:	out std_logic_vector(7 downto 0)
	);
end Encoder;

architecture logic of Encoder is

	-- Counter value
	--signal Count: std_logic_vector(7 downto 0) := (others => '0');
	
	-- Contains the last input value that is different
	signal Last_Val: std_logic_vector(1 downto 0);
	
begin

	process(Input, RST)
		
		variable Count: std_logic_vector(7 downto 0) := (others => '0');
		
		begin
		
		if RST = '1' then
			Count := (others => '0');
			Last_Val <= (others => '0');
		elsif Input /= Last_Val then
			
			-- Loop around positive direction - Count up
			if Input = "00" and Last_val = "11" then
				Count := Count + '1';
				
			-- Loop around negative direction - Count down
			elsif Input = "11" and Last_val = "00" then
				Count := Count - '1';
			
			-- Count down
			elsif Input < Last_Val then
				Count := Count - '1';
			
			-- Count up
			elsif Input > Last_Val then
				Count := Count + '1';
			end if;
			
			Last_Val <= Input;
			
		end if;
		Output <= Count;
	end process;
	
	Test(1 downto 0) <= Last_Val;
	
end logic;

