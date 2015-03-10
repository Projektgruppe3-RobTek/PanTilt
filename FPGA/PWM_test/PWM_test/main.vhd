------------
--  main  --
------------
library ieee;
use ieee.std_logic_1164.all;

entity main is
	port(
		BTN: in std_logic_vector(3 downto 0);
		Clock: in std_logic;
		SW: in std_logic_vector(7 downto 0);
		IN1A : out std_logic;
		IN2A : out std_logic;
		IN1B : out std_logic;
		IN2B : out std_logic;
		ENA  : out std_logic;
		ENB  : out std_logic;
		HS2B : in std_logic;
		HS2A : in std_logic;
		HS1b : in std_logic;
		HS1A : in std_logic;
		I0   : in std_logic;
		I1   : in std_logic;
		LD: out std_logic_vector(7 downto 0)
	);
end main;

architecture behav of main is
signal JA : STD_LOGIC_VECTOR(3 downto 0);




	
	component SignedPWM is
	
		generic(
			constant SignedBitWidth: positive := 8
		);
	
		port(
			CounterClock: in std_logic;
			CompareMatch: in std_logic_vector((SignedBitWidth - 1) downto 0);
			Output: out std_logic_vector(1 downto 0)
		);
	
	end component;
	
	component Memory1Byte is
		port(
			EI, R, EO: in std_logic;
			I: in std_logic_vector(7 downto 0);
			O: out std_logic_vector(7 downto 0)
		);
	end component;
	
	signal Motor1, Motor2, sEI: std_logic_vector(1 downto 0);
	signal Speed1, Speed2: std_logic_vector(7 downto 0);
	
	begin
	IN1A <= JA(0);
IN2A <= JA(1);
IN1B <= JA(2);
IN2B <= JA(3);
ENA <= '1';
ENB <= '1';
	
	sEI(0) <= BTN(0) and (not BTN(3));
	sEI(1) <= BTN(1) and (not BTN(3));
	
	--LD <=
	--	Speed1 when BTN(0) = '1' else
	--	Speed2 when BTN(1) = '1' else
	--	"00000000";
	
	LD(0) <= HS1A;
	LD(1) <= HS2A;
	LD(2) <= HS1B;
	ld(3) <= HS2B;
	LD(4) <= I0;
	LD(5) <= I1;
	LD(6) <= '0';
	LD(7) <= '0';
	
	JA(1 downto 0) <= Motor1;
	JA(3 downto 2) <= Motor2;
	
	PWM1: SignedPWM
		generic map(
			SignedBitWidth => 8
		)
		port map(
			CounterClock => Clock,
			CompareMatch => speed1,
			Output => Motor1
		)
	;
	
	PWM2: SignedPWM
		generic map(
			SignedBitWidth => 8
		)
		port map(
			CounterClock => Clock,
			CompareMatch => speed2,
			Output => Motor2
		)
	;
	
	Mem1: Memory1Byte port map(
		EI => sEI(0),
		R => BTN(2),
		EO => '1',
		I => SW,
		O => Speed1
	);
	
	Mem2: Memory1Byte port map(
		EI => sEI(1),
		R => BTN(2),
		EO => '1',
		I => SW,
		O => Speed2
	);
	
end behav;
