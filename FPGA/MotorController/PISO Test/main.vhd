------------
--  main  --
------------
library ieee;
use ieee.std_logic_1164.all;

entity main is
	port(
		-- 
		CLK: in std_logic;			-- FPGA Clock
		
		SW: in std_logic_vector(7 downto 0);	-- Test Input
		
		-- SPI0
		JC1: in std_logic;  -- SPI0CLK
		JC2: in std_logic;  -- SPI0SS
		JC3: in std_logic;  -- SPI0MOSI
		JC4: out std_logic  -- SPI0MISO
	);
end main;

architecture logic of main is
	
	component PISO is
		generic(
			constant ENCBitWidth	:	positive := 8;	-- Size of the input vector
			constant TimeBitWidth	:	positive := 8	-- Size of the input vector
		);
		port(
			RST	:	in  std_logic;
		
			Latch	:	in  std_logic;
			CLK	:	in  std_logic;
		
			PIENC	:	in  std_logic_vector(ENCBitWidth-1 downto 0);
			PITime	:	in  std_logic_vector(TimeBitWidth-1 downto 0);
		
			SO	:	out std_logic
		);
	end component;
	
	component Pulser is
		port(
			RST		:	in  std_logic;
			CLK		: 	in  std_logic;
			
			Input		:	in  std_logic;
			Output		:	out std_logic
		);
	end component;
	
	signal sPISOLatch, sSS: std_logic;
	
begin
	sSS <= not JC2;
	Pulser0: Pulser
	port map(
		RST => '0',
		CLK => CLK,
		
		Input => sSS,
		Output => sPISOLatch
	);
	
	PISO0: PISO
	generic map(
		ENCBitWidth => 4,
		TimeBitWidth => 4
	)
	port map(
		RST => '0',
		
		Latch => sPISOLatch,
		CLK => JC1,
		
		PIENC => SW(7 downto 4),
		PITime => SW(3 downto 0),
		
		SO => JC4
	);
	
end logic;
