---------------------------
--                       --
--   MotorController     --
--                       --
---------------------------
library ieee;
use ieee.std_logic_1164.all;

entity MotorController is
	generic(
		-- PWM constants
		constant PWMBitWidth		:	positive := 8;	-- 8-bit signed PWM
		constant PWMCLKScale		:	positive := 5;	-- 50MHz / (PWMPrescaler * 2) | (5MHz)
									-- 50MHz / (PWMPrescaler * 2 * 252) | (20KHz)
		-- ENC constants
		constant ENCCLKScale		:	positive := 25;	-- 50MHz / (ENCTimePrescaler * 2) | (1000KHz)
		constant ENCTimeBitWidth	:	positive := 8;	-- 8-bit timer
		constant ENCCountBitWidth	:	positive := 8	-- 8-bit Encoder value
	);
	port(
		-- 
		RST		:	in  std_logic;	-- Reset
		CLK		:	in  std_logic;	-- Clock
		
		-- SPI signals
		SPICLK		:	in  std_logic;	-- SPI Clock
		SPISS		:	in  std_logic;	-- SPI Slave Select
		SPIMOSI		:	in  std_logic;	-- SPI MOSI
		SPIMISO		:	out std_logic;	-- SPI MISO
		
		-- Read/Write test output
		RW		:	out std_logic;	-- Read/Write
		
		-- PWM signals
		PWMOutput	:	out std_logic_vector(1 downto 0);	-- PWM Motor Output

		-- ENC signals
		ENCInput	:	in  std_logic_vector(1 downto 0)	-- Encoder Input
	);
end MotorController;

architecture logic of MotorController is
	
----------   Components   ----------
	
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
			PI		: 	in  std_logic_vector(PISOBitWidth-1 downto 0);
			
			-- Parallel output
			PO		:	out std_logic_vector(SIPOBitWidth-1 downto 0)
		);
	end component;
	
	component PWMController is
		generic(
			-- PWM Prescaler value
			constant CLKScale	:	positive := 5;	-- Prescaled Clock = 50MHz / (PWMClockPrescaler * 2)
									-- PWMClockPrescaler -> 50MHz / (5 * 2) = 5MHz;
			-- PWM compare match bit width
			constant BitWidth	:	positive := 8	-- PWM Frequency = 50MHz / (PWMClockPrescaler * 2 * (2 ** PWMBitWidth) - 4)
									-- PWM Frequency -> 50MHz / (5 * 2 * (2 ** 8) - 4) = 20KHz
		);
		port(
			RST		:	in  std_logic;
			CLK		:	in  std_logic;
		
			PWMCM		:	in  std_logic_vector((BitWidth - 1) downto 0);
		
			PWMOutput	:	out std_logic_vector(1 downto 0)
		);
	end component;
	
	component ENCController is
		generic(
			constant CLKScale	:	positive := 25; -- 50MHz / (Scale * 2) = 1MHz
			constant TimeBitWidth	:	positive := 8;	-- Bit width of timer counter
			constant CountBitWidth	:	positive := 8	-- Encoder counter bit width
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
	
	-- PWM compare match signal
	signal sPWMCM		:	std_logic_vector(PWMBitWidth-1 downto 0);
	
	-- ENC value signal
	signal sENCCount	:	std_logic_vector(ENCCountBitWidth-1 downto 0);
	
	-- ENC time signal
	signal sENCTime		:	std_logic_vector(ENCTimeBitWidth-1 downto 0);
	
	-- Combined ENC time and ENC count
	signal sENCOutput	:	std_logic_vector(ENCCountBitWidth+ENCTimeBitWidth-1 downto 0);
	
	-- ENC reset signal
	signal sENCRST		:	std_logic;
	
	-- SPI PISOLatch signal
	signal sPISOLatch	:	std_logic;
	
	begin
	
	RW <= sPISOLatch;
	
	sENCRST <= RST or sPISOLatch;
	
	sENCOutput(ENCCountBitWidth+ENCTimeBitWidth-1 downto ENCTimeBitWidth) <= sENCCount;
	sENCOutput(ENCTimeBitWidth-1 downto 0) <= sENCTime;
	
	SPI: SPIController
	generic map(
		SIPOBitWidth => PWMBitWidth,
		PISOBitWidth => ENCCountBitWidth+ENCTimeBitWidth
	)
	port map(
		
		RST => RST,
		CLK => CLK,
		
		SPICLK => SPICLK,
		SPISS => SPISS,
		SPIMOSI => SPIMOSI,
		SPIMISO => SPIMISO,
		
		RW => sPISOLatch,
		
		PI => sENCOutput,
		PO => sPWMCM
	);
	
	PWM: PWMController
	generic map(
		BitWidth => PWMBitWidth,
		CLKScale => PWMCLKScale
	)
	port map(
		
		RST => RST,
		CLK => CLK,
		
		PWMCM => sPWMCM,
		
		PWMOutput => PWMOutput
	);
	
	ENC: EncController
	generic map(
		CLKScale => ENCCLKScale,
		TimeBitWidth => ENCTimeBitWidth,
		CountBitWidth => ENCCountBitWidth
	)
	port map(
		
		RST => sENCRST,
		CLK => CLK,
		
		ENCInput => ENCInput,
		
		ENCCount => sENCCount,
		ENCTime => sENCTime
	);
	
end logic;
