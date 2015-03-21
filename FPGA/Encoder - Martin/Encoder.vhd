library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity Encoder is
	generic(
		EncoderBitWidth: integer := 8
	);	
	port(
		CLK		 :  in  std_logic;
		RST		 :  in  std_logic;
		Input		 :	 in  std_logic_vector(1 downto 0);
		Output	 :	 out std_logic_vector(EncoderBitWidth-1 downto 0)
	);
end Encoder;

architecture logic of Encoder is

	-- Counter value
	signal Count: std_logic_vector(EncoderBitWidth-1 downto 0) := (others => '0');
	
	-- Contains the last input value that is different
	signal LastInput: std_logic_vector(1 downto 0);
	
begin
	
	process(CLK) begin
		if rising_edge(CLK) then
			if RST = '1' then
				Count <= (others => '0');
				
			elsif Input /= LastInput then
			
				LastInput <= Input;
			
				-- Loop around positive direction - Count up
				if Input = "00" and LastInput = "11" then
					Count <= Count + '1';
					
				-- Loop around negative direction - Count down
				elsif Input = "11" and LastInput = "00" then
					Count <= Count - '1';
				
				-- Count down
				elsif Input < LastInput then
					Count <= Count - '1';
				
				-- Count up
				elsif Input > LastInput then
					Count <= Count + '1';
				end if;
			end if;
		end if;
		
		
		if Count(EncoderBitWidth-1) = '1' then
			Output(EncoderBitWidth-2 downto 0) <= not Count(EncoderBitWidth-2 downto 0);
			Output(EncoderBitWidth-1) <= Count(EncoderBitWidth-1);
		else
			Output <= Count;
		end if;
		
	end process;
end logic;

