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
		JC4: out std_logic;  -- SPI0MISO
		
		JC7: out std_logic	-- Test
	);
end main;

architecture logic of main is
	
	component SPIController is
		generic(
			constant PWMBitWidth	:	positive := 8;	-- Size of the output vector
			constant ENCBitWidth	:	positive := 8;	-- Size of the input vector
			constant TimeBitWidth	:	positive := 8	-- Size of the input vector
		);
		port(
			RST		:	in  std_logic;
			CLK		:	in  std_logic;
			
			-- SPI signals
			SPICLK		:	in  std_logic;
			SPISS		:	in  std_logic;
			SPIMOSI		:	in  std_logic;
			SPIMISO		:	out std_logic;
			
			-- Test
			ReadWrite	:	out std_logic;
			
			-- ENC signals
			ENCVal		: 	in  std_logic_vector(ENCBitWidth-1 downto 0);
			ENCTime		:	in  std_logic_vector(TimeBitWidth-1 downto 0);
			
			-- PWM signals
			PWMCompareMatch	:	out std_logic_vector(PWMBitWidth-1 downto 0)
		);
	end component;
	
begin
	
	SPIController0: SPIController
	generic map(
		PWMBitWidth => 8,
		ENCBitWidth => 4,
		TimeBitWidth => 4
	)
	port map(
		RST => '0',
		CLK => CLK,
		
		SPICLK => JC1,
		SPISS => JC2,
		SPIMOSI => JC3,
		SPIMISO => JC4,
		
		ENCVal => SW(7 downto 4),
		ENCTime => SW(3 downto 0),
		
		ReadWrite => JC7,
		
		PWMCompareMatch => LD
	);
	
end logic;
