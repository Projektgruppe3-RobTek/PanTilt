-----------------
--  SignedPWM  --
-----------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_arith.all;

entity SignedPWM is
	
	generic(
		-- define bit width of pwm
		constant SignedBitWidth: positive := 8
	);
	
	port(
		-- clock impulse to count
		CounterClock: in std_logic;
		-- compare match input vector
		CompareMatch: in std_logic_vector((SignedBitWidth - 1) downto 0);
		-- pwm output
		Output: out std_logic_vector(1 downto 0)
	);
	
end SignedPWM;

architecture behav of SignedPWM is
	
	component UnsignedPWM is
		
		generic(
			-- define bit width of pwm
			constant UnsignedBitWidth: positive := 7
		);
		
		port(
			-- clock impulse to count
			CounterClock: in std_logic;
			-- compare match input vector
			CompareMatch: in std_logic_vector((UnsignedBitWidth - 1) downto 0);
			-- pwm output
			Output: out std_logic
		);
		
	end component;
	
	signal sCounterClock: std_logic;
	signal sCompareMatch: std_logic_vector((SignedBitWidth - 2) downto 0);
	signal sUnsignedOutput: std_logic;
	
	begin
	
	sCounterClock <= CounterClock;
	
	-- set unsigned compare match for unsigned pwm controller
	
	sCompareMatch <=
		CompareMatch((SignedBitWidth - 2) downto 0) when CompareMatch(SignedBitWidth - 1) = '0' else
		not CompareMatch((SignedBitWidth - 2) downto 0);
	
	-- implement unsigned pwm component
	
	PWM: UnsignedPWM
		generic map(
			UnsignedBitWidth => SignedBitWidth - 1
		)
		port map(
			CounterClock => sCounterClock,
			CompareMatch => sCompareMatch((SignedBitWidth - 2) downto 0),
			Output => sUnsignedOutput
		)
	;
	
	-- set outputs
	
	Output(0) <= sUnsignedOutput when (CompareMatch(SignedBitWidth - 1) = '1') else '0';
	Output(1) <= sUnsignedOutput when (CompareMatch(SignedBitWidth - 1) = '0') else '0';
	
end behav;
