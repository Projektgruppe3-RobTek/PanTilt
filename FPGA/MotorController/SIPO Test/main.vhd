------------
--  main  --
------------
library ieee;
use ieee.std_logic_1164.all;

entity main is
	port(
		CLK: in std_logic;			-- FPGA Clock
		
		SW: in std_logic_vector(7 downto 0);	-- Test Input
		LD: out std_logic_vector(7 downto 0);	-- Test Output
		
		-- SPI0
		JC1: in std_logic;  -- SPI0CLK
		JC2: in std_logic;  -- SPI0SS
		JC3: in std_logic;  -- SPI0MOSI
		JC4: out std_logic  -- SPI0MISO
	);
end main;

architecture logic of main is
	
	component SIPO is
		generic(
			constant BitWidth	:	positive := 8	-- Size of the output vector
		);
		port(
			RST		:	in  std_logic;
			
			Latch		: 	in  std_logic;
			CLK		: 	in  std_logic;
			SI		: 	in  std_logic;
			
			ReadWrite	: 	out std_logic;
			PO		:	out std_logic_vector(BitWidth-1 downto 0)
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
	
	signal sCLK, sSIPOLatch: std_logic;
	
begin
	JC4 <= JC3;
	
	Pulser0: Pulser
	port map(
		RST => '0',
		CLK => CLK,
		
		Input => JC1,
		Output => sSIPOLatch
	);
	
	SIPO0: SIPO
	generic map(
		BitWidth => 7
	)
	port map(
		RST => '0',
		
		CLK => JC1,
		Latch => sSIPOLatch,
		SI => JC3,
		
		ReadWrite => LD(0),
		
		PO => LD(7 downto 1)
	);
	
end logic;
