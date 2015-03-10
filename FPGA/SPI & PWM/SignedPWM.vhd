-----------------
--  SignedPWM  --
-----------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_arith.all;

entity SignedPWM is
	
	generic(
		constant SignedBitWidth: positive := 8
	);
	
	port(
		CounterClock: in std_logic;
		CompareMatch: in std_logic_vector((SignedBitWidth - 1) downto 0);
		Output: out std_logic_vector(1 downto 0)
	);
	
end SignedPWM;

architecture behav of SignedPWM is
	
	component UnsignedPWM is
		
		generic(
			constant UnsignedBitWidth: positive := 7
		);
		
		port(
			CounterClock: in std_logic;
			CompareMatch: in std_logic_vector((UnsignedBitWidth - 1) downto 0);
			Output: out std_logic
		);
		
	end component;
	
	signal sCounterClock: std_logic;
	signal sCompareMatch: std_logic_vector((SignedBitWidth - 2) downto 0);
	signal sUnsignedOutput: std_logic;
	
	begin
	
	sCounterClock <= CounterClock;
	sCompareMatch <=
		CompareMatch((SignedBitWidth - 2) downto 0) when CompareMatch(SignedBitWidth - 1) = '0' else
		not CompareMatch((SignedBitWidth - 2) downto 0);
	
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
	
	Output(0) <= sUnsignedOutput when (CompareMatch(SignedBitWidth - 1) = '1') else '0';
	Output(1) <= sUnsignedOutput when (CompareMatch(SignedBitWidth - 1) = '0') else '0';
	
end behav;
