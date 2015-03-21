---------------------
--  PWMController  --
---------------------
library ieee;
use ieee.std_logic_1164.all;

entity PWMController is
	generic(
		constant PWMBitWidth: positive := 8;		-- Bit width of signed compareMatch
		constant PWMClockPrescaler: positive := 50	-- 50MHz / scale = 1000Hz
	);
	port(
		-- 
		Reset: in std_logic;
		Clock: in std_logic;
		
		-- Input
		PWMCompareMatch: in std_logic_vector(7 downto 0);
		
		-- Output
		PWMOutput: out std_logic_vector(1 downto 0)
	);
end PWMController;

architecture behav of PWMController is
	
	component PWM is
		generic(
			constant BitWidth: positive := 8	-- Bit width of unsigned compareMatch
		);
		port(
			-- 
			Reset: in std_logic;
			Clock: in std_logic;
			
			-- Input
			CompareMatch: in std_logic_vector((BitWidth - 1) downto 0);
			
			-- Output
			Output: out std_logic
		);
	end component;
	
	component Prescaler is
		generic(
			constant Scale: positive := 50000 -- 50MHz / scale = 1000Hz
		);
		port(
			Reset: in std_logic;
			Clock: in std_logic;
			PrescaledClock: out std_logic
		);
	end component;
	
	signal sReset: std_logic;
	signal sClock: std_logic;
	
	signal sUnsignedOutput: std_logic;
	signal sPrescaledClock: std_logic;
	
	signal sUnsignedComapreMatch: std_logic_vector(6 downto 0);
	
	begin
	
	sReset <= Reset;
	sClock <= Clock;
	
	sUnsignedComapreMatch <=
		PWMCompareMatch((8 - 2) downto 0) when PWMCompareMatch(8 - 1) = '0' else
		not PWMCompareMatch((8 - 2) downto 0);
		--PWMCompareMatch((BitWidth - 2) downto 0) when PWMCompareMatch(BitWidth - 1) = '0' else
		--not PWMCompareMatch((BitWidth - 2) downto 0);
	
	Prescaler0: Prescaler
	generic map(
		Scale => PWMClockPrescaler -- 50MHz / scale = 1000Hz
	)
	port map(
		Reset => sReset,
		Clock => sClock,
		PrescaledClock => sPrescaledClock
	);
	
	PWM0: PWM
	generic map(
		BitWidth => PWMBitWidth - 1
	)
	port map(
		Reset => sReset,
		Clock => sPrescaledClock,
		
		CompareMatch => sUnsignedComapreMatch,
		Output => sUnsignedOutput
	);
	
	PWMOutput(0) <= sUnsignedOutput when (PWMCompareMatch(8 -1) = '1') else '0';
	PWMOutput(1) <= sUnsignedOutput when (PWMCompareMatch(8 -1) = '0') else '0';
	--PWMOutput(0) <= sUnsignedOutput when (PWMCompareMatch(BitWidth -1) = '1') else '0';
	--PWMOutput(1) <= sUnsignedOutput when (PWMCompareMatch(BitWidth -1) = '0') else '0';
	
end behav;
