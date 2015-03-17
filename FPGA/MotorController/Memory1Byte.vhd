-------------------
--  Memory1Byte  --
-------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Memory1Byte is
	port(
		EI, R, EO: in std_logic;
		I: in std_logic_vector(7 downto 0);
		O: out std_logic_vector(7 downto 0)
	);
end Memory1Byte;

architecture behav of Memory1Byte is
	
	component RS_NorLatch is
		port(
			S, R: in std_logic;
			Q: out std_logic
		);
	end component;
	
	signal sS, sR, sO: std_logic_vector(7 downto 0);
	
	begin
	
	sS(0) <= I(0) and EI;
	sS(1) <= I(1) and EI;
	sS(2) <= I(2) and EI;
	sS(3) <= I(3) and EI;
	sS(4) <= I(4) and EI;
	sS(5) <= I(5) and EI;
	sS(6) <= I(6) and EI;
	sS(7) <= I(7) and EI;
	
	sR(0) <= (not I(0) and EI) or R;
	sR(1) <= (not I(1) and EI) or R;
	sR(2) <= (not I(2) and EI) or R;
	sR(3) <= (not I(3) and EI) or R;
	sR(4) <= (not I(4) and EI) or R;
	sR(5) <= (not I(5) and EI) or R;
	sR(6) <= (not I(6) and EI) or R;
	sR(7) <= (not I(7) and EI) or R;
	
	O <= sO when EO = '1' else "00000000";
	
	Latch0: RS_NorLatch port map(S => sS(0), R => sR(0), Q => sO(0));
	Latch1: RS_NorLatch port map(S => sS(1), R => sR(1), Q => sO(1));
	Latch2: RS_NorLatch port map(S => sS(2), R => sR(2), Q => sO(2));
	Latch3: RS_NorLatch port map(S => sS(3), R => sR(3), Q => sO(3));
	Latch4: RS_NorLatch port map(S => sS(4), R => sR(4), Q => sO(4));
	Latch5: RS_NorLatch port map(S => sS(5), R => sR(5), Q => sO(5));
	Latch6: RS_NorLatch port map(S => sS(6), R => sR(6), Q => sO(6));
	Latch7: RS_NorLatch port map(S => sS(7), R => sR(7), Q => sO(7));
	
end behav;
