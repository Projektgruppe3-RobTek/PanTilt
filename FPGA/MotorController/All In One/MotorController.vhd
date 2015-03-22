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
		constant PWMBitWidth	:	positive := 8;	-- 8-bit signed PWM
		constant PWMCLKScale	:	positive := 5;	-- 50MHz / (PWMPrescaler * 2) | (5MHz)
									-- 50MHz / (PWMPrescaler * 2 * 252) | (20KHz)
		-- ENC constants
		constant TimeBitWidth	:	positive := 8;	-- 8-bit timer
		constant ENCCLKScale	:	positive := 25;	-- 50MHz / (ENCTimePrescaler * 2) | (1000KHz)
		constant ENCBitWidth	:	positive := 8	-- 8-bit Encoder value
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
			constant PWMBitWidth		:	positive := 8;	-- Size of the output vector
			constant ENCBitWidth		:	positive := 8;	-- Size of the input vector
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
			PISOLatch	:	out std_logic;
			
			-- ENC signals
			ENCVal		: 	in  std_logic_vector(ENCBitWidth-1 downto 0);
			ENCTime		:	in  std_logic_vector(TimeBitWidth-1 downto 0);
			
			-- PWM signals
			PWMCompareMatch	:	out std_logic_vector(PWMBitWidth-1 downto 0)
		);
	end component;
	
	component PWMController is
		generic(
			-- PWM Prescaler value
			constant PWMCLKScale	:	positive := 5;	-- Prescaled Clock = 50MHz / (PWMClockPrescaler * 2)
									-- PWMClockPrescaler -> 50MHz / (5 * 2) = 5MHz;
			-- PWM compare match bit width
			constant PWMBitWidth	:	positive := 8	-- PWM Frequency = 50MHz / (PWMClockPrescaler * 2 * (2 ** PWMBitWidth) - 4)
									-- PWM Frequency -> 50MHz / (5 * 2 * (2 ** 8) - 4) = 20KHz
		);
		port(
			RST		:	in  std_logic;
			CLK		:	in  std_logic;
		
			PWMCompareMatch	:	in  std_logic_vector((PWMBitWidth - 1) downto 0);
			
			PWMOutput	:	out std_logic_vector(1 downto 0)
		);
	end component;
	
	component ENCController is
		generic(
			constant ENCCLKScale	:	positive := 25; -- 50MHz / (Scale * 2) = 1MHz
			constant TimeBitWidth	:	positive := 8;	-- Bit width of timer counter
			constant ENCBitWidth	:	positive := 8	-- Encoder counter bit width
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
	
	-- PWM compare match signal
	signal sPWMCM		:	std_logic_vector((PWMBitWidth - 1) downto 0);
	
	-- ENC value signal
	signal sENCCount		:	std_logic_vector((ENCBitWidth - 1) downto 0);
	
	-- ENC time signal
	signal sENCTime		:	std_logic_vector((TimeBitWidth - 1) downto 0);
	
	-- ENC reset signal
	signal sENCRST		:	std_logic;
	
	-- SPI PISOLatch signal
	signal sPISOLatch	:	std_logic;
	
	begin
	
	sENCRST <= RST or sPISOLatch;
	
	SPI: SPIController
	generic map(
		PWMBitWidth => PWMBitWidth,
		TimeBitWidth => TimeBitWidth,
		ENCBitWidth => ENCBitWidth
	)
	port map(
		
		RST => RST,
		CLK => CLK,
		
		SPICLK => SPICLK,
		SPISS => SPISS,
		SPIMOSI => SPIMOSI,
		SPIMISO => SPIMISO,
		
		PISOLatch => sPISOLatch,
		
		ENCTime => sENCTime,
		ENCVal => sENCCount,
		
		PWMCompareMatch => sPWMCM
	);
	
	PWM: PWMController
	generic map(
		PWMBitWidth => PWMBitWidth,
		PWMCLKScale => PWMCLKScale
	)
	port map(
		
		RST => RST,
		CLK => CLK,
		
		PWMCompareMatch => sPWMCM,
		
		PWMOutput => PWMOutput
	);
	
	ENC: EncController
	generic map(
		ENCCLKScale => ENCCLKScale,
		TimeBitWidth => TimeBitWidth,
		ENCBitWidth => ENCBitWidth
	)
	port map(
		
		RST => sENCRST,
		CLK => CLK,
		
		ENCTime => sENCTime,
		ENCCount => sENCCount,
		
		ENCInput => ENCInput
	);
	
end logic;
