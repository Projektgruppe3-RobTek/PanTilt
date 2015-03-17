-----------------------
--     Debounce      --
-----------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Debounce is
	generic(
		counter_size: integer := 19 -- counter size (19 bits gives 10.5ms with 50MHz clock)
	); 
	port(
		CLK, input: in std_logic;
		output: out std_logic
	);
end Debounce;

architecture logic of debounce is
	signal counter_set :	 std_logic;
	signal input_delay :	 std_logic_vector(1 downto 0); -- Used to check if the input changes
	signal counter 	 :  std_logic_vector(counter_size downto 0) := (others => '0');
begin

	counter_set <= input_delay(0) xor input_delay(1); -- Check if input changed
  
	process(CLK) begin
		if(CLK'event and CLK = '1') then
			input_delay(0) <= input;
			input_delay(1) <= input_delay(0);
			
			if(counter_set = '1') then -- reset if input is changing
				counter <= (others => '0');
			elsif(counter(counter_size) = '0') then -- count if button have not been hold down for the set amout of time
				counter <= counter + 1;
			else -- if the counter have reached MSB = 1 then the output is set high
				output <= input_delay(0) and input_delay(1);
			end if;    
		end if;
	end process;
end logic;
