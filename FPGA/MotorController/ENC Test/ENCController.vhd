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
		constant CLKScale	:	positive := 25; -- 50MHz / (Scale * 2) = 1MHz
		constant TimeBitWidth	:	positive := 8;
		constant ENCBitWidth	:	positive := 8
	);
	port(
		--
		RST		:	in  std_logic;
		CLK		:	in  std_logic;
		
		-- Input
		ENCInput	:	in  std_logic_vector(1 downto 0);
			
		TestOutput	:	out std_logic;
		
		-- Output
		ENCCount	:	out std_logic_vector(ENCBitWidth-1 downto 0);
		ENCTime		:	out std_logic_vector(TimeBitWidth-1 downto 0)
	);
end ENCController;

architecture logic of ENCController is
	
----------   Components   ----------
	
	component Encoder is
		generic(
			constant EncoderBitWidth	:	positive := 8 -- Size of the output vector
		);	
		port(
			RST	:	in  std_logic;
			CLK	:	in  std_logic;
		
			Input	:	in  std_logic_vector(1 downto 0);
			Output	:	out std_logic_vector(EncoderBitWidth-1 downto 0)
		);
	end component;
	
	component Prescaler is
		generic(
			constant Scale	:	positive := 25 -- 50MHz / (Scale * 2) = 1MHz
		);
		port(
			RST		:	in  std_logic;
			CLK		:	in  std_logic;
		
			ScaledCLK	:	out std_logic
		);
	end component;
	
	component Counter is
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
	
	-- Scaled Clock
	signal sCLK	:	std_logic;
	
begin
	
	TestOutput <= sCLK;
	
	Encoder0: Encoder
	generic map(
		EncoderBitWidth => ENCBitWidth
	)
	port map(
		RST => RST,
		CLK => CLK,
		
		Input => ENCInput,
		Output => ENCCount
	);
	
	Prescaler0: Prescaler
	generic map(
		Scale => CLKScale
	)
	port map(
		RST => RST,
		CLK => CLK,
		
		ScaledCLK => sCLK
	);
	
	Timer: Counter
	generic map(
		BitWidth => TimeBitWidth
	)
	port map(
		RST => RST,
		CLK => sCLK,
		
		Output => ENCTime
	);
	
end logic;
