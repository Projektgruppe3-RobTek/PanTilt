-----------------------
--  ClockPrescaler  --
-----------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity ClockPrescaler is

    port(
    		prescaler: in std_logic_vector (14 downto 0) ;
        clk_in: in std_logic;
        scaled_clk: out std_logic
    );
end ClockPrescaler;

architecture logic of ClockPrescaler is

   signal prescaler_counter: std_logic_vector(14 downto 0) := (others => '0');
   signal new_clk : std_logic := '0';

	begin

   process(clk_in, new_clk) begin
       if (clk_in'event and clk_in = '1') then
           prescaler_counter <= prescaler_counter + 1;
           if(prescaler_counter > prescaler) then
               new_clk <= not new_clk;
               prescaler_counter <= (others => '0');
           end if;
       end if;
   end process;
	 
	scaled_clk <= new_clk;

end logic;
