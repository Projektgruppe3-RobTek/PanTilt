------------
--  PISO  -- SIPO
------------
library ieee;
use ieee.std_logic_1164.all;

entity PISO is
	generic(
		constant Data_Width: positive := 8
	);
	port(
		CLK: in std_logic;
		RST: in std_logic;
		LATCH: in std_logic;
		Input: in std_logic_vector((8 - 1) downto 0);
		Debug: out std_logic_vector((8 - 1) downto 0);
		Output: out std_logic
	);
end PISO;

architecture logic of PISO is
	
	component D_FlipFlop is
		port(
			D: in std_logic;
			CLK: in std_logic;
			RST: in std_logic;
			Q: out std_logic
		);
	end component;
	
	signal sClock, sReset: std_logic;
	signal sSerialInput, sSerialOutput: std_logic_vector(8 downto 0);
	
	begin
	
	sReset <= RST;
	sClock <= CLK or LATCH;
	Output <= sSerialOutput(7);
	Debug <= sSerialOutput(7 downto 0);
	
	SignalGenerator:
	for i in 0 to 8 - 1 generate
		Start: if (i = 0) generate
			signalIn(0) <= (LATCH and Input(0));
		end generate Start;
		
		Rest: if (i > 0) generate
			signalIn(i) <= (CLK and signalOut(i - 1)) or (LATCH and Input(i));
		end generate Rest;
	end generate SignalGenerator;
	
	FlipFlopGenerator:
	for i in 0 to 8 - 1 generate
		D_FlipFlopX: D_FlipFlop port map(
			D => signalIn(i),
			CLK => signalCLK,
			RST => signalRST,
			Q => signalOut(i)
		);
	end generate FlipFlopGenerator;
end logic;
