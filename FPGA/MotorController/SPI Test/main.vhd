------------
--  main  --
------------
library ieee;
use ieee.std_logic_1164.all;

entity main is
	port(
		CLK	:	in  std_logic;				-- FPGA Clock
		
		BTN	:	in  std_logic_vector(0 downto 0);	-- Reset
		SW	:	in  std_logic_vector(7 downto 0);	-- Test Input
		LD	:	out std_logic_vector(7 downto 0);	-- Test Output
		
		-- SPI0
		JC1	:	in  std_logic;	-- SPI0CLK
		JC2	:	in  std_logic;	-- SPI0SS
		JC3	:	in  std_logic;	-- SPI0MOSI
		JC4	:	out std_logic;	-- SPI0MISO
		
		JD1	:	out std_logic	-- RW
	);
end main;

architecture logic of main is
	
	component SPIController is
		generic(
			constant PISOBitWidth	:	positive := 8;	-- Size of the parallel input vector
			constant SIPOBitWidth	:	positive := 8	-- Size of the parallel output vector
		);
		port(
			RST		:	in  std_logic;
			CLK		:	in  std_logic;
			
			-- SPI signals
			SPICLK		:	in  std_logic;
			SPISS		:	in  std_logic;
			SPIMOSI		:	in  std_logic;
			SPIMISO		:	out std_logic;
			
			-- Read/Write signal
			RW		:	out std_logic;
			
			-- Parallel input
			PI		: 	in  std_logic_vector(SIPOBitWidth-1 downto 0);
			
			-- Parallel output
			PO		:	out std_logic_vector(SIPOBitWidth-1 downto 0)
		);
	end component;
	
begin
	
	SPIController0: SPIController
	generic map(
		SIPOBitWidth => 8,
		PISOBitWidth => 8
	)
	port map(
		RST => BTN(0),
		CLK => CLK,
		
		SPICLK => JC1,
		SPISS => JC2,
		SPIMOSI => JC3,
		SPIMISO => JC4,
		
		RW => JD1,
		
		PI => SW,
		PO => LD
	);
	
end logic;
