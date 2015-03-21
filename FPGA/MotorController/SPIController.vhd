---------------------
--  SPIController  --
---------------------
library ieee;
use ieee.std_logic_1164.all;

entity SPIController is
	generic(
		constant PWMBitWidth: positive := 8;
		constant ENCTimeBitWidth: positive := 8;
		constant ENCValBitWidth: positive := 8
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
end SPIController;

architecture behav of SPIController is
	
	component SIPO is
		generic(
			constant BitWidth: positive := 8
		);
		port(
			Reset: in std_logic;
			Clock: in std_logic;
			
			Latch: in std_logic;
			
			SerialInput: in std_logic;
			SerialOutput: out std_logic;
			
			ParallelOutput: out std_logic_vector((BitWidth - 1) downto 0)
		);
	end component;
	
	component PISO is
		generic(
			constant PISOBitWidth: positive := 8
		);
		port(
			Reset: in std_logic;
			Clock: in std_logic;
			
			Latch: in std_logic;
			
			SerialInput: in std_logic;
			SerialOutput: out std_logic;
			
			ParallelInput: in std_logic_vector((PISOBitWidth - 1) downto 0)
		);
	end component;
	
	signal sReset: std_logic;
	signal sClock: std_logic;
	signal sSIPOLatch: std_logic;
	signal sPISOLatch: std_logic;
	signal sReadWrite: std_logic;
	
	signal sParallelOutput: std_logic_vector((PWMBitWidth) downto 0);
	
	begin
	
	sReset <= Reset;
	sClock <= Clock and (not SPISS);
	
	sSIPOLatch <= SPISS;
	sPISOLatch <= sReadWrite and (not SPISS);
	
	PWMCompareMatch <= sParallelOutput((PWMBitWidth - 2) downto 0);
	sReadWrite <= sParallelOutput(PWMBitWidth - 1);
	
	SIPO0: SIPO
	generic map(
		BitWidth => PWMBitWidth + 1
	)
	port map(
		Reset => sReset,
		Clock => sClock,
		
		Latch => sSIPOLatch,
		
		SerialInput => SPIMOSI,
		SerialOutput => open,
		ParallelOutput => sParallelOutput
	);
	
	PISO0: PISO
	generic map(
		PISOBitWidth => ENCTimeBitWidth + ENCValBitWidth
	)
	port map(
		Reset => sReset,
		Clock => sClock,
		
		Latch => sPISOLatch,
		
		SerialInput => '0',
		SerialOutput => SPIMISO,
		
		--ParallelInput((ENCTimeBitWidth + ENCValBitWidth - 1) downto ENCValBitWidth) => ENCTime,
		--ParallelInput((ENCValBitWidth - 1) downto 0) => ENCVal
		ParallelInput((8 + 8 - 1) downto 8) => ENCTime,
		ParallelInput((8 - 1) downto 0) => ENCVal
	);
	
end behav;
