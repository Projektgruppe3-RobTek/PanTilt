---------------------------
--                       --
--     ENCController     --
--                       --
---------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity ENCController is
	generic(
		constant TimeBitWidth	:	positive := 8;	-- Bit width of timer counter
		constant CountBitWidth	:	positive := 8	-- Encoder counter bit width
	);
	port(
		--
		RST		:	in  std_logic;
		CLK		:	in  std_logic;

		-- Input
		ENCInput	:	in  std_logic_vector(1 downto 0);


		-- Output
		ENCCount	:	out std_logic_vector(CountBitWidth-1 downto 0);
		ENCTime		:	out std_logic_vector(TimeBitWidth-1 downto 0)
	);
end ENCController;

architecture logic of ENCController is

----------   Components   ----------

	component Encoder is
		generic(
			constant BitWidth	:	positive := 8 -- Size of the output vector
		);
		port(
			RST	:	in  std_logic;
			CLK	:	in  std_logic;

			Input	:	in  std_logic_vector(1 downto 0);
			Output	:	out std_logic_vector(BitWidth-1 downto 0)
		);
	end component;

	component Timer is
		generic(
			constant BitWidth	:	positive := 8 -- Bit width of counter
		);
		port(
			RST	:	in std_logic;
			CLK	:	in std_logic;

			Output	:	out std_logic_vector(BitWidth-1 downto 0)
		);
	end component;

----------   Signals   ----------

begin

	Encoder0: Encoder
	generic map(
		BitWidth => CountBitWidth
	)
	port map(
		RST => RST,
		CLK => CLK,

		Input => ENCInput,
		Output => ENCCount
	);
	
	Timer: Timer
	generic map(
		BitWidth => TimeBitWidth
	)
	port map(
		RST => RST,
		CLK => CLK,

		Output => ENCTime
	);

end logic;
