------------
--  main  --
------------
library ieee;
use ieee.std_logic_1164.all;

entity main is
	port(
		-- 
		BTN: in std_logic_vector(2 downto 0);	-- Reset button
		CLK: in std_logic;			-- FPGA Clock
		
		SW: in std_logic_vector(7 downto 0);	-- Test Input
		LD: out std_logic_vector(7 downto 0)	-- Test Output
		
		
		
		-- SPI0
		--JC1: in std_logic;  -- SPI0CLK
		--JC2: in std_logic;  -- SPI0SS
		--JC3: in std_logic;  -- SPI0MOSI
		--JC4: out std_logic  -- SPI0MISO
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
	
	component Debounce is
		generic(
			counter_size: integer := 19 -- counter size (19 bits gives 10.5ms with 50MHz clock)
		); 
		port(
			clk_in, BTN: in std_logic;
			result: out std_logic
		);
	end component;
	
	signal BTN0, BTN1, BTN2	:	std_logic;
	signal sLoopBack	:	std_logic;
	
begin
	
	Debounce0: Debounce port map(clk_in => CLK, BTN => BTN(0), result => BTN0);
	Debounce1: Debounce port map(clk_in => CLK, BTN => BTN(1), result => BTN1);
	Debounce2: Debounce port map(clk_in => CLK, BTN => BTN(2), result => BTN2);
	
	SPIController0: SPIController
	generic map(
		PWMBitWidth => 7,
		ENCBitWidth => 6,
		TimeBitWidth => 1
	)
	port map(
		RST => BTN2,
		CLK => CLK,
		
		SPICLK => BTN0,
		SPISS => SW(0),
		SPIMOSI => sLoopBack,
		SPIMISO => sLoopBack,
		
		ENCVal => SW(7 downto 2),
		ENCTime => SW(1 downto 1),
		
		ReadWrite => LD(0),
		
		PWMCompareMatch => LD(7 downto 1)
	);
	
end logic;
