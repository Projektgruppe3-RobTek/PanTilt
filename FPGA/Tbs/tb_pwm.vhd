--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   22:21:05 03/15/2015
-- Design Name:   
-- Module Name:   C:/Users/nwl1515/Dropbox/RobTek - F15 Projekt/DIG/Kode/Tbs/Tbs/tb_pwm.vhd
-- Project Name:  Tbs
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: SignedPWM
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
use ieee.numeric_std.all;
use ieee.std_logic_arith.all;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY tb_pwm IS
END tb_pwm;
 
ARCHITECTURE behavior OF tb_pwm IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT SignedPWM
    PORT(
         CounterClock : IN  std_logic;
         CompareMatch : IN  std_logic_vector(7 downto 0);
         Output : OUT  std_logic_vector(1 downto 0)
        );
    END COMPONENT;
    

   --Inputs
   signal CounterClock : std_logic := '0';
   signal CompareMatch : std_logic_vector(7 downto 0) := (others => '0');

 	--Outputs
   signal Output : std_logic_vector(1 downto 0) := "00";

 
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: SignedPWM PORT MAP (
          CounterClock => CounterClock,
          CompareMatch => CompareMatch,
          Output => Output
        );

 CounterClock <= not CounterClock after 1 ns; 
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
      wait for 100 ns;

	CompareMatch <= "01000000";
		
		wait for 100 ns;
		CompareMatch <= "10000000";
	

      

      -- insert stimulus here 

      wait;
   end process;

END;
