-----------------------
--  MotorController  --
-----------------------
library ieee;
use ieee.std_logic_1164.all;

entity MotorController is
	generic(
		-- PWM constants
		constant ControllerPWMBitWidth: positive := 8;		-- 8-bit signed PWM
		constant ControllerPWMPrescaler: positive := 5;		-- 50MHz / (PWMPrescaler * 2) | (5MHz)
									-- 50MHz / (PWMPrescaler * 2 * 252) | (20KHz)
		-- ENC constants
		constant ControllerENCTimeBitWidth: positive := 8;	-- 8-bit timer
		constant ControllerENCTimePrescaler: positive := 25;	-- 50MHz / (ENCTimePrescaler * 2) | (1000KHz)
		constant ControllerENCValBitWidth: positive := 8	-- 8-bit Encoder value
	);
	port(
		-- 
		Reset: in std_logic;	-- Reset
		Clock: in std_logic;	-- Clock
		
		-- SPI signals
		SPICLK: in std_logic;	-- SPI Clock
		SPISS: in std_logic;	-- SPI Slave Select
		SPIMOSI: in std_logic;	-- SPI MOSI
		SPIMISO: out std_logic;	-- SPI MISO

		-- PWM signals
		PWMOutput: out std_logic_vector(1 downto 0);	-- PWM Motor Output
		PWMEN: out std_logic;				-- H-Bridge Enable

		-- ENC signals
		ENCInput: in std_logic_vector(1 downto 0)	-- Encoder Input
	);
end MotorController;

architecture behav of MotorController is
	
	component SPIController is
		generic(
			constant PWMBitWidth: positive := 8;			-- Bit width of signed compareMatch
			constant ENCTimeBitWidth: positive := 8;		-- Bit width of encoder timer
			constant ENCValBitWidth: positive := 8			-- Bit width of signed encoder value
		);
		port(
			-- 
			Reset: in std_logic;
			Clock: in std_logic;
			
			-- SPI signals
			SPICLK: in std_logic;
			SPISS: in std_logic;
			SPIMOSI: in std_logic;
			SPIMISO: out std_logic;
			
			-- PWM signals
			PWMCompareMatch: out std_logic_vector((PWMBitWidth - 1) downto 0);
			
			-- ENC signals
			ENCTime: in std_logic_vector((ENCTimeBitWidth - 1) downto 0);
			ENCVal: in std_logic_vector((ENCValBitWidth - 1) downto 0)
		);
	end component;
	
	component PWMController is
		generic(
			constant PWMBitWidth: positive := 8;		-- Bit width of signed compareMatch
			constant PWMClockPrescaler: positive := 25	-- 50MHz / (PWMClockPrescaler * 2) | 1000KHz
		);
		port(
			-- 
			Reset: in std_logic;
			Clock: in std_logic;
			
			-- Input
			PWMCompareMatch: in std_logic_vector((PWMBitWidth - 1) downto 0);
			
			-- Output
			PWMOutput: out std_logic_vector(1 downto 0)
		);
	end component;
	
	component ENCController is
		generic(
			constant ENCTimeBitWidth: positive := 8;	-- Bit width of encoder timer
			constant PWMClockPrescaler: positive := 25;	-- 50MHz / (scale * 2) | 1000KHz
			constant ENCValBitWidth: positive := 8		-- Bit width of signed encoder value
		);
		port(
			--
			Reset: in std_logic;
			Clock: in std_logic;
			
			-- Output
			ENCTime: out std_logic_vector((ENCTimeBitWidth - 1) downto 0);
			ENCVal: out std_logic_vector((ENCValBitWidth - 1) downto 0);
			
			-- Input
			ENCInput: in std_logic_vector(1 downto 0)
		);
	end component;
	
	-- 
	signal sReset: std_logic;
	signal sClock: std_logic;
	
	-- SPI signals
	signal sSPICLK: std_logic;
	signal sSPISS: std_logic;
	signal sSPIMOSI: std_logic;
	signal sSPIMISO: std_logic;
	
	-- PWM signals
	signal sPWMOutput: std_logic_vector(1 downto 0);
	signal sPWMCompareMatch: std_logic_vector((ControllerPWMBitWidth - 1) downto 0);
	
	-- ENC signals
	signal sENCInput: std_logic_vector(1 downto 0);
	signal sENCTime: std_logic_vector((ControllerENCTimeBitWidth - 1) downto 0);
	signal sENCVal: std_logic_vector((ControllerENCValBitWidth - 1) downto 0);
	
	begin
	
	SPI: SPIController
	generic map(
		PWMBitWidth => ControllerPWMBitWidth,
		ENCTimeBitWidth => ControllerENCTimeBitWidth,
		ENCValBitWidth => ControllerENCValBitWidth
	)
	port map(
		
		Reset => sReset,
		Clock => sClock,
		
		SPICLK => sSPICLK,
		SPISS => sSPISS,
		SPIMOSI => sSPIMOSI,
		SPIMISO => sSPIMISO,
		
		PWMCompareMatch => sPWMCompareMatch,
		
		ENCTime => sENCTime,
		ENCVal => sENCVal
	);
	
	PWM: PWMController
	generic map(
		PWMBitWidth => ControllerPWMBitWidth,
		PWMClockPrescaler => ControllerPWMPrescaler
	)
	port map(
		
		Reset => sReset,
		Clock => sClock,
		
		PWMCompareMatch => sPWMCompareMatch,
		
		PWMOutput => sPWMOutput
	);
	
	Enc: EncController
	generic map(
		ENCTimeBitWidth => ControllerENCTimeBitWidth,
		PWMClockPrescaler => ControllerPWMPrescaler,
		ENCValBitWidth => ControllerENCValBitWidth
	)
	port map(
		
		Reset => sReset,
		Clock => sClock,
		
		ENCTime => sENCTime,
		ENCVal => sENCVal,
		
		ENCInput => sENCInput
	);
	
	-- 
	sReset <= Reset;
	sClock <= Clock;
	
	-- SPI signals
	sSPICLK <= SPICLK;
	sSPISS <= SPISS;
	sSPIMOSI <= SPIMOSI;
	SPIMISO <= sSPIMISO;
	
	-- PWM signals
	PWMOutput <= sPWMOutput;
	PWMEN <= '1';
	
	-- ENC signals
	sENCInput <= ENCInput;
	
end behav;
