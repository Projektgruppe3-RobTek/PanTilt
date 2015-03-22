---------------------
--  SPIController  --
---------------------
library ieee;
use ieee.std_logic_1164.all;

entity SPIController is
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
			
		-- PISOLatch to Encoder reset
		PISOLatch	:	out std_logic;
				
		-- ENC signals
		ENCVal		: 	in  std_logic_vector(ENCBitWidth-1 downto 0);
		ENCTime		:	in  std_logic_vector(TimeBitWidth-1 downto 0);
		
		-- PWM signals
		PWMCompareMatch	:	out std_logic_vector(PWMBitWidth-1 downto 0)
	);
end SPIController;

architecture logic of SPIController is

----------   Components   ----------

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
	
----------   Signals   ----------
	
	signal sCLK		:	std_logic;
	
	signal sPISOPulseInput	:	std_logic;
	
	signal sSIPOLatch	:	std_logic;
	signal sPISOLatch	:	std_logic;
	
	signal sReadWrite	:	std_logic;
	
begin
	
	-- PISOLatch to Encoder reset
	PISOLatch <= sPISOLatch;
	
	-- Input to PISOLatch pulser
	sPISOPulseInput <= not sReadWrite and not SPISS;
	
	-- When ss is high the clock is disabled
	sCLK <= SPICLK and not SPISS;
	
	Pulser0: Pulser
	port map(
		RST => '0',
		CLK => CLK,
		
		Input => SPISS,
		Output => sSIPOLatch
	);
	
	SIPO0: SIPO
	generic map(
		BitWidth => PWMBitWidth
	)
	port map(
		RST => '0',
		
		Latch => sSIPOLatch,
		CLK => sCLK,
		
		SI => SPIMOSI,
		
		ReadWrite => sReadWrite,
		
		PO => PWMCompareMatch
	);
	
	Pulser1: Pulser
	port map(
		RST => RST,
		CLK => CLK,
		
		Input => sPISOPulseInput,
		Output => sPISOLatch
	);
	
	PISO0: PISO
	generic map(
		ENCBitWidth => ENCBitWidth,
		TimeBitWidth => TimeBitWidth
	)
	port map(
		RST => RST,
		
		Latch => sPISOLatch,
		CLK => sCLK,
		
		PIENC => ENCVal,
		PITime => ENCTime,
		
		SO => SPIMISO
	);	
end logic;
