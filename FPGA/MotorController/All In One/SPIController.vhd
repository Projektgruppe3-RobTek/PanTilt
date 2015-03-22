-------------------------
--                     --
--    SPIController    --
--                     --
-------------------------
library ieee;
use ieee.std_logic_1164.all;

entity SPIController is
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
		PI		: 	in  std_logic_vector(PISOBitWidth-1 downto 0);
		
		-- Parallel output
		PO		:	out std_logic_vector(SIPOBitWidth-1 downto 0)
	);
end SPIController;

architecture logic of SPIController is

----------   Components   ----------
	
	component PISO is
		generic(
			constant BitWidth	:	positive := 8	-- Size of the input vector
		);
		port(
			RST	:	in  std_logic;
			
			LATCH	:	in  std_logic;
			CLK	:	in  std_logic;
			
			SI	:	in  std_logic := '0';
			PI	:	in  std_logic_vector(BitWidth-1 downto 0);
			SO	:	out std_logic
		);
	end component;

	component SIPO is
		generic(
			constant BitWidth	:	positive := 8	-- Size of the output vector
		);
		port(
			RST		:	in  std_logic;
			
			LATCH		: 	in  std_logic;
			CLK		: 	in  std_logic;
			
			RW		: 	out std_logic;
			
			SO		: 	out std_logic;
			SI		: 	in  std_logic;
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
	
----------   Signals   ----------
	
	signal sCLK		:	std_logic;
	
	signal sPISOPulse	:	std_logic;
	
	signal sSIPOLATCH	:	std_logic;
	signal sPISOLATCH	:	std_logic;
	
	signal sRW		:	std_logic;
	
begin
	
	-- Read/Write output
	RW <= sRW;
	
	-- Input to PISOLatch pulser
	sPISOPulse <= not SPISS and not sRW;
	
	-- When ss is high the clock is disabled
	sCLK <= not SPISS and SPICLK;
	
	PISOPulser0: Pulser
	port map(
		RST => RST,
		CLK => CLK,
		
		Input => sPISOPulse,
		Output => sPISOLATCH
	);
	
	PISO0: PISO
	generic map(
		BitWidth => PISOBitWidth
	)
	port map(
		RST => RST,
		
		LATCH => sPISOLATCH,
		CLK => sCLK,
		
		PI => PI,
		SO => SPIMISO
	);
	
	SIPOPulser0: Pulser
	port map(
		RST => RST,
		CLK => CLK,
		
		Input => SPISS,
		Output => sSIPOLATCH
	);
	
	SIPO0: SIPO
	generic map(
		BitWidth => SIPOBitWidth
	)
	port map(
		RST => RST,
		
		Latch => sSIPOLATCH,
		CLK => sCLK,
		
		RW => sRW,
		
		SI => SPIMOSI,
		PO => PO
	);	
end logic;
