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
		BTN	:	in  std_logic_vector(1 downto 0);	-- Reset button and latch button
		
		SW	:	in  std_logic_vector(1 downto 0);	-- Sensor Input
		LD	:	out std_logic_vector(7 downto 0)	-- Encoder Output
		
	);
end main;

architecture logic of main is
	
----------   Components   ----------
	
	component ENCController is
		generic(
			constant CLKScale	:	positive := 25; -- 50MHz / (Scale * 2) = 1MHz
			constant TimeBitWidth	:	positive := 8;
			constant CountBitWidth	:	positive := 8
		);
		port(
			--
			RST		:	in  std_logic;
			CLK		:	in  std_logic;
		
			-- Input
			ENCInput	:	in  std_logic_vector(1 downto 0);
			
			-- Output
			ENCCount	:	out std_logic_vector(CountBitWidth-1 downto 0);
			ENCTime		:	out std_logic_vector(TimeBitWidth-1 downto 0)
		);
	end component;
	
----------   Signals   ----------
	
	-- SW test inputs
	signal sTest	:	std_logic_vector(1 downto 0);
	
	begin
	
	ENCController0: ENCController
	generic map(
		CLKScale => 25000000,
		TimeBitWidth => 4,
		CountBitWidth => 4
	)
	port map(
		RST => BTN(1),
		CLK => CLK,
		
		ENCInput => sTest,
		
		ENCCount => LD(7 downto 4),
		ENCTime => LD(3 downto 0)
	);
	
	-- Latched input
	Process(BTN(0)) begin
		if rising_edge(BTN(0)) then
			sTest <= SW;	
		end if;
	end process;
end logic;
