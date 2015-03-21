------------
--  main  --
------------
library ieee;
use ieee.std_logic_1164.all;

entity main is
	port(
		-- 
		BTN: in std_logic_vector(0 downto 0);	-- Reset button
		Clock: in std_logic;			-- FPGA Clock
		
		SW: in std_logic_vector(7 downto 0);	-- Test Input
		LD: out std_logic_vector(7 downto 0);	-- Test Output
		
		-- SPI0
		JC1: in std_logic;  -- SPI0CLK
		JC2: in std_logic;  -- SPI0SS
		JC3: in std_logic;  -- SPI0MOSI
		JC4: out std_logic  -- SPI0MISO
	);
end main;

architecture behav of main is
	
	component SIPO is
		generic(
			constant PWMBitWidth: positive := 8
		);
		port(
			Reset: in std_logic;
			Clock: in std_logic;
		
			Latch: in std_logic;
		
			SerialInput: in std_logic;
			
			ReadWrite: out std_logic;
			
			ParallelOutput: out std_logic_vector((BitWidth - 1) downto 0)
		);
	end component;
	
	component PISO is
		generic(
			constant ENCTimeBitWidth: positive := 8;
			constant ENCValBitWidth: positive := 8
		);
		port(
			Reset: in std_logic;
			Clock: in std_logic;
		
			Latch: in std_logic;
		
			SerialOutput: out std_logic;
		
			ParallelInput: in std_logic_vector((BitWidth - 1) downto 0)
		);
	end component;
	
	signal sClock, sSIPOLatch, sPISOLatch, sReadWrite: std_logic;
	
	begin
	
	sClock <= Clock and (not JC2);
	sSIPOLatch <= JC2;
	sPISOLatch <= JC2 nor sReadWrite;
	
	SIPO0: SIPO
	generic map(
		PWMBitWidth => 8
	)
	port map(
		Reset => BTN(0),
		Clock => sClock,
		
		Latch => sSIPOLatch,
		
		SerialInput => JC3,
		
		ReadWrite => sReadWrite,
		
		ParallelOutput => LD
	);
	
	PISO0: PISO
	generic map(
		ENCTimeBitWidth => 8,
		ENCValBitWidth => 8
	)
	port map(
		Reset => BTN(0),
		Clock => sClock,
		
		Latch => sPISOLatch,
		
		SerialOutput => JC4,
		
		ParallelInput => SW
	);
	
end behav;
