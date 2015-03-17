------------
--  main  --
------------
library ieee;
use ieee.std_logic_1164.all;

entity main is
	port(
		BTN: in std_logic_vector(3 downto 0);
		SW: in std_logic_vector(7 downto 0);
		LD: out std_logic_vector(7 downto 0);
		DP: out std_logic_vector(7 downto 0);
		AN: out std_logic_vector(3 downto 0);
		
		JA2, JA3, JA4, JA8, JA9, JA10: in std_logic;
		
		JB1, JB2, JB3, JB7, JB8, JB9: out std_logic;
		
		JC1, JC2, JC3, JC7: in std_logic;
		JC4: out std_logic;
		
		Clock: in std_logic
	);
end main;

architecture behav of main is
	
	component DisplayDriver is
		port(
			I0, I1: in std_logic_vector(7 downto 0);
			C: in std_logic;
			DP_Output: out std_logic_vector(7 downto 0);
			AN_Output: out std_logic_vector(3 downto 0)
		);
	end component;
	
	component Debounce is
		generic(
			CounterSize: integer := 19
		); 
		port(
			CLK, input: in std_logic;
			output: out std_logic
		);
	end component;
	
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
	
	component ScalerStefan is
		generic (SCALE: integer range 0 to 500000000); --what is the clock scaled down by.
		port(
			IN_SIG: IN STD_LOGIC;
			OUT_SIG: OUT STD_LOGIC
		);
	end component;
	
	signal BTN0, BTN1, BTN2, BTN3: std_logic;
	
	signal Motor1, Motor2: std_logic_vector(1 downto 0);
	signal Speed1, Speed2: std_logic_vector(7 downto 0);
	
	signal Reset, sClock: std_logic;
	
	begin
	
	Reset <= '1';
	
	LD(0) <= JA9;		-- HS1A
	LD(1) <= JA4;		-- HS2A
	LD(2) <= JA3;		-- HS1B
	LD(3) <= JA10;		-- HS2B
	LD(4) <= JA2;		-- Index0
	LD(5) <= JA8;		-- Index1
	LD(6) <= '0';
	LD(7) <= '0';
	
	--LD <= Speed1 when BTN3 = '0' else Speed2;
	
	JB8 <= '1';		-- ENA
	JB1 <= '1';		-- ENB
	
	JB3 <= Motor1(0);	-- IN1A
	JB9 <= Motor1(1);	-- IN2A
	JB2 <= Motor2(0);	-- IN1B
	JB7 <= Motor2(1);	-- IN2B
	
	JC4 <= JC7; -- MISO <= S
	
	ScalerStefan0: ScalerStefan
		generic map(
			SCALE => 8
		)
		port map(
			in_sig => Clock,
			out_sig => sClock
		);
	PWM1: SignedPWM
		generic map(
			SignedBitWidth => 8
		)
		port map(
			CounterClock => sClock,
			CompareMatch => Speed1,
			Output => Motor1
		)
	;
	
	PWM2: SignedPWM
		generic map(
			SignedBitWidth => 8
		)
		port map(
			CounterClock => sClock,
			CompareMatch => Speed2,
			Output => Motor2
		)
	;
	
	Speed1 <= "00100000" when BTN(0) = '1' and BTN(1) = '1' else
		"11011111" when BTN(0) = '1' and BTN(1) = '0' else "00000000";
	Speed2 <= "00100000" when BTN(3) = '1' and BTN(2) = '1' else
		"11011111" when BTN(3) = '1' and BTN(2) = '0' else "00000000";
	
	DisplayDriver0: DisplayDriver port map(
		I0 => Speed1,
		I1 => Speed2,
		C => Clock,
		DP_Output => DP,
		AN_Output => AN
	);
	
--	Debounce0: Debounce port map(CLK => Clock, Input => BTN(0), Output => BTN0);
--	Debounce1: Debounce port map(CLK => Clock, Input => BTN(1), Output => BTN1);
--	Debounce2: Debounce port map(CLK => Clock, Input => BTN(2), Output => BTN2);
--	Debounce3: Debounce port map(CLK => Clock, Input => BTN(3), Output => BTN3);
	
end behav;
