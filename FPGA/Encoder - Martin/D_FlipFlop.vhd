library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity D_FlipFlop is
	port(
		D: in std_logic;
		--S: in std_logic;
		R: in std_logic;
		--RST: in std_logic;
		CLK: in std_logic;
		Q: out std_logic
	);
end D_FlipFlop;

architecture logic of D_FlipFlop is
begin

	process(CLK, R) begin
		if rising_edge(R) then
			Q <= '0';
		elsif rising_edge(CLK) then
			Q <= D;
		end if;
	end process;

	--Q <= Reset;
--	process(S, Reset, CLK) begin
--		if Reset = '1' then
--			Q <= '0';
--		elsif rising_edge(S) then
--			Q <= '1';
--		elsif rising_edge(CLK) then
--			Q <= D;
--		end if;
--	end process;
end logic;
