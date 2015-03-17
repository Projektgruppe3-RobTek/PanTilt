--------------
--  Memory  --
--------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Memory is
	generic(
		constant BitWidth: positive := 8
	);
	port(
		EI, R, EO: in std_logic;
		Input: in std_logic_vector(7 downto 0);
		O: out std_logic_vector(7 downto 0)
	);
end Memory;

architecture behav of Memory is
	
	component RS_NorLatch is
		port(
			S, R: in std_logic;
			Q: out std_logic
		);
	end component;
	
	signal sS, sR, sO: std_logic_vector(7 downto 0);
	
	begin
	
	-- generate set-signals
	SetGen: for i in 0 to (BitWidth - 1) generate
		sS(i) <= Input(i) and EI;
	end generate SetGen;
	
	-- generate reset signals
	ResetGen: for i in 0 to (BitWidth - 1) generate
		sR(i) <= (not Input(i) and EI) or R;
	end generate ResetGen;
	
	-- generate latches
	LatchGen: for i in 0 to (BitWidth - 1) generate
		LatchX: RS_NorLatch port map(S => sS(i), R => sR(i), Q => sO(i));
	end generate LatchGen;
	
	O <= sO when EO = '1' else "00000000";
	
end behav;
