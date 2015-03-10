------------
--  PISO  -- SIPO
------------
library ieee;
use ieee.std_logic_1164.all;

entity SIPO is
	generic(
		constant DataWidth: positive := 8
	);
	port(
		CLK: in std_logic;
		RST: in std_logic;
		Input: in std_logic;
		Latch: in std_logic;
		Output: out std_logic_vector((DataWidth - 1) downto 0)
	);
end SIPO;

architecture logic of SIPO is
	
	component D_FlipFlop is
		port(
			D: in std_logic;
			CLK: in std_logic;
			RST: in std_logic;
			Q: out std_logic
		);
	end component;
	
	signal Reset, Clock: std_logic;
	signal ParallelOutput: std_logic_vector((DataWidth - 1) downto 0);
	
	begin
	
	Reset <= RST;
	Clock <= CLK and not Latch;
	
	ShiftRegisterGenerator:
	for i in 0 to (DataWidth - 1) generate
		start: if (i = 0) generate
			D_FlipFlopX: D_FlipFlop port map(
				D => Input,
				CLK => Clock,
				RST => Reset,
				Q => ParallelOutput(0)
			);
		end generate start;
		
		rest: if (i > 0) generate
			D_FlipFlopX: D_FlipFlop port map(
				D => ParallelOutput(i - 1),
				CLK => Clock,
				RST => Reset,
				Q => ParallelOutput(i)
			);
		end generate rest;
		
	end generate ShiftRegisterGenerator;
	
	LatchGenerator:
	for i in 0 to (DataWidth - 1) generate
		D_FlipFlopX: D_FlipFlop port map(
			D => ParallelOutput(i),
			CLK => Latch,
			RST => Reset,
			Q => Output(i)
		);
	end generate LatchGenerator;
end logic;
