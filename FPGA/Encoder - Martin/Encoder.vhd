library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity Encoder is
	generic(
		Size	: 	integer := 8
	);	
	port(
		CLK		 :  in  std_logic;
		RST		 :  in  std_logic;
		Input		 :	 in  std_logic_vector(1 downto 0);
		Output	 :	 out std_logic_vector(Size-1 downto 0);
		Timer_Out :	 out std_logic_vector(Size-1 downto 0)
	);
end Encoder;

architecture logic of Encoder is

	component Counter is
		generic(
			Size	: 	integer := 4
		);
		port(
			RST			: 	in  std_logic;
			Input			: 	in  std_logic;
			MAX			: 	in  std_logic_vector(size-1 downto 0);
			Output		: 	out std_logic_vector(size-1 downto 0); 
			Carry_Out	: 	out std_logic
		);
	end component;

	-- Counter value
	signal Count: std_logic_vector(Size-1 downto 0) := (others => '0');
	
	-- Contains the last input value that is different
	signal Last_Val: std_logic_vector(1 downto 0);
	
begin
	
	-- Timer
	Timer: Counter generic map(Size => Size) port map(Input => CLK, RST => RST, MAX => "11111111", Output => Timer_Out);
	
	-- Encoder
	process(CLK) begin
		if rising_edge(CLK) then
			if RST = '1' then
				Count <= (others => '0');
				Last_Val <= (others => '0');
				
			elsif Input /= Last_Val then
			
				Last_Val <= Input;
			
				-- Loop around positive direction - Count up
				if Input = "00" and Last_val = "11" then
					Count <= Count + '1';
					
				-- Loop around negative direction - Count down
				elsif Input = "11" and Last_val = "00" then
					Count <= Count - '1';
				
				-- Count down
				elsif Input < Last_Val then
					Count <= Count - '1';
				
				-- Count up
				elsif Input > Last_Val then
					Count <= Count + '1';
				end if;
			end if;
		end if;
		
		Output <= Count;
		
	end process;
end logic;

