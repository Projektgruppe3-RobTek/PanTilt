---------------------
--  PWMController  --
---------------------
library ieee;
use ieee.std_logic_1164.all;

entity PWMController is
	generic(
		-- PWM Prescaler value
		constant PWMClockPrescaler: positive := 5;	-- Prescaled Clock = 50MHz / (PWMClockPrescaler * 2)
								-- PWMClockPrescaler -> 50MHz / (5 * 2) = 5MHz;
		-- PWM compare match bit width
		constant PWMBitWidth: positive := 8		-- PWM Frequency = 50MHz / (PWMClockPrescaler * 2 * (2 ** PWMBitWidth) - 4)
								-- PWM Frequency -> 50MHz / (5 * 2 * (2 ** 8) - 4) = 20KHz
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
end PWMController;

architecture behav of PWMController is
	
	component PWM is
		generic(
			constant BitWidth: positive := 8	-- PWM Frequency = 50MHz / (PWMClockPrescaler * 2 * (2 ** PWMBitWidth) - 4)
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
			constant Scale: positive := 50000	-- Prescaled Clock = 50MHz / (PWMClockPrescaler * 2)
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
	
	signal sUnsignedComapreMatch: std_logic_vector((PWMBitWidth - 2) downto 0);
	
	begin
	
	sReset <= Reset;
	sClock <= Clock;
	
	sUnsignedComapreMatch <=
		PWMCompareMatch((PWMBitWidth - 2) downto 0) when PWMCompareMatch(PWMBitWidth - 1) = '0' else
		not PWMCompareMatch((PWMBitWidth - 2) downto 0);
	
	Prescaler0: Prescaler
	generic map(
		Scale => PWMClockPrescaler
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
	
	PWMOutput(0) <= sUnsignedOutput when (PWMCompareMatch(PWMBitWidth -1) = '1') else '0';
	PWMOutput(1) <= sUnsignedOutput when (PWMCompareMatch(PWMBitWidth -1) = '0') else '0';
	
end behav;
