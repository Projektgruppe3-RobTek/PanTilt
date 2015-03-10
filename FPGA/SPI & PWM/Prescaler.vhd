-----------------
--  Prescaler  --
-----------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.std_logic_signed.all;

entity Prescaler is
	port(
		I, R: in std_logic;
		S: in std_logic_vector(3 downto 0);
		O: out std_logic
	);
end Prescaler;

architecture behav of Prescaler is
	
	component T_FlipFlop is
		port(
			T, S, R: in std_logic;
			Q, IQ: out std_logic
		);
	end component;
	
	signal state, Istate: std_logic_vector(15 downto 0);
	
	signal sI, sR: std_logic;
	
	begin
	
	sI <= I;
	sR <= R;
	O <= state(conv_integer(S));
	
	flipFlop0: T_FlipFlop port map(T => sI, S => '0', R => sR, Q => state(0), IQ => Istate(0));
	flipFlop1: T_FlipFlop port map(T => Istate(0), S => '0', R => sR, Q => state(1), IQ => Istate(1));
	flipFlop2: T_FlipFlop port map(T => Istate(1), S => '0', R => sR, Q => state(2), IQ => Istate(2));
	flipFlop3: T_FlipFlop port map(T => Istate(2), S => '0', R => sR, Q => state(3), IQ => Istate(3));
	flipFlop4: T_FlipFlop port map(T => Istate(3), S => '0', R => sR, Q => state(4), IQ => Istate(4));
	flipFlop5: T_FlipFlop port map(T => Istate(4), S => '0', R => sR, Q => state(5), IQ => Istate(5));
	flipFlop6: T_FlipFlop port map(T => Istate(5), S => '0', R => sR, Q => state(6), IQ => Istate(6));
	flipFlop7: T_FlipFlop port map(T => Istate(6), S => '0', R => sR, Q => state(7), IQ => Istate(7));
	flipFlop8: T_FlipFlop port map(T => Istate(7), S => '0', R => sR, Q => state(8), IQ => Istate(8));
	flipFlop9: T_FlipFlop port map(T => Istate(8), S => '0', R => sR, Q => state(9), IQ => Istate(9));
	flipFlop10: T_FlipFlop port map(T => Istate(9), S => '0', R => sR, Q => state(10), IQ => Istate(10));
	flipFlop11: T_FlipFlop port map(T => Istate(10), S => '0', R => sR, Q => state(11), IQ => Istate(11));
	flipFlop12: T_FlipFlop port map(T => Istate(11), S => '0', R => sR, Q => state(12), IQ => Istate(12));
	flipFlop13: T_FlipFlop port map(T => Istate(12), S => '0', R => sR, Q => state(13), IQ => Istate(13));
	flipFlop14: T_FlipFlop port map(T => Istate(13), S => '0', R => sR, Q => state(14), IQ => Istate(14));
	flipFlop15: T_FlipFlop port map(T => Istate(14), S => '0', R => sR, Q => state(15), IQ => Istate(15));
	
end behav;
