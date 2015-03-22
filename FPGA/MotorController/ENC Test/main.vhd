---------------------------
--                       --
--         main          --
--                       --
---------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity main is
	port(
		-- 
		CLK	:	in  std_logic;				-- FPGA Clock
		BTN	:	in  std_logic_vector(1 downto 0);	-- Reset button
		
		SW	:	in  std_logic_vector(1 downto 0);	-- Sensor Input
		LD	:	out std_logic_vector(7 downto 0)	-- Sensor Output
		
	);
end main;

architecture logic of main is
	
----------   Components   ----------
	
	component ENCController is
		generic(
			constant ENCCLKScale		:	positive := 25; -- 50MHz / (Scale * 2) = 1MHz
			constant TimeBitWidth	:	positive := 8;
			constant ENCBitWidth		:	positive := 8
		);
		port(
			--
			RST		:	in  std_logic;
			CLK		:	in  std_logic;
		
			-- Input
			ENCInput	:	in  std_logic_vector(1 downto 0);
			
			-- Output
			ENCCount	:	out std_logic_vector(ENCBitWidth-1 downto 0);
			ENCTime		:	out std_logic_vector(TimeBitWidth-1 downto 0)
		);
	end component;
	
----------   Signals   ----------
	signal Temp	:	std_logic_vector(1 downto 0);
	
	begin
	
	ENCController0: ENCController
	generic map(
		ENCCLKScale => 25000000,
		TimeBitWidth => 4,
		ENCBitWidth => 4
	)
	port map(
		RST => BTN(1),
		CLK => CLK,
		
		ENCInput => Temp,
		
		ENCCount => LD(7 downto 4),
		ENCTime => LD(3 downto 0)
	);
	
	-- Latched input
	Process(BTN(0), BTN(1)) begin
		if rising_edge(BTN(0)) then
			Temp <= SW;	
		end if;
	end process;
end logic;
