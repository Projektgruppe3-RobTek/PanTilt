----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    23:11:28 02/09/2015 
-- Design Name: 
-- Module Name:    Clock_adjuster - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Clock_adjuster is
	PORT ( 	clk_50MHz : in STD_LOGIC;
				clk_24800Hz : out STD_LOGIC);
end Clock_adjuster;

architecture Behavioral of Clock_adjuster is

	signal	clk_1000Hz_i : std_logic := '0';
	begin

	adjust : process (clk_50MHz,sel)
	variable counter : Integer := 0;
	variable top	  : Integer := 50000000/24800 -1;
-------
	
--------	
	begin
	
	
----------	
	if rising_edge(clk_50MHz) then
	if (clk_1000Hz_i = '1') then
		clk_1000Hz_i <= not clk_1000Hz_i;
		counter := counter + 1;
		
	elsif ( counter > top) then
			counter := 1;
			clk_1000Hz_i <= not clk_1000Hz_i;
	else
			counter := counter + 1;
	end if;
	end if;
	end process adjust;
	
	
	
clk_1000Hz <= clk_1000Hz_i;
	
	
end Behavioral;