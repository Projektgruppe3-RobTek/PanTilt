-- Roundaboutsomething --
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.all;

entity StopWatch is

	port (
			CLK	: 	IN  STD_LOGIC;
			SW		: 	IN  STD_LOGIC_VECTOR(1 downto 0);
			BTN	: 	IN  STD_LOGIC_VECTOR(3 downto 0);
			LD		:	OUT STD_LOGIC_VECTOR(1 downto 0);
			DP		: 	OUT STD_LOGIC_VECTOR(6 downto 0);
			AN		: 	OUT STD_LOGIC_VECTOR(3 downto 0) -- Used to select a segment on the display
	);
end StopWatch;

architecture logic of StopWatch is

----------    Encoder     ----------
	component Encoder is
		generic(
			EncoderBitWidth	: 	integer := 8
		);	
		port(
			CLK			:  in	 std_logic;
			RST			: 	in  std_logic;
			Input			:	in  std_logic_vector(1 downto 0);
			Output		:	out std_logic_vector(EncoderBitWidth-1 downto 0)
		);
	end component;

	signal Encoder_Val: std_logic_vector(7 downto 0);
	signal Encoder_Timer: std_logic_vector(7 downto 0);
	signal Encoder_Test: std_logic_vector(1 downto 0);

----------   T_FlipFlop   ----------
	component T_FlipFlop is
		port(
			input	 	: 	in std_logic;
			output 	: 	out std_logic
		);
	end component;
	
	signal state: std_logic_vector(1 downto 0);
	signal lap_toggle_state, stop_toggle_state: std_logic;

-------------   Memory   -------------	
	component Memory is
		port(
			input: in std_logic_vector(1 downto 0);
			SET, RST, clk_in: in std_logic;
			output: out std_logic_vector(1 downto 0)
		);
	end component;
	
	signal memory_storage: std_logic_vector(1 downto 0);

--------    Display_Driver    --------
	component Display_Driver is
		port(
			input		: 	in std_logic_vector(3 downto 0);
			output	: 	out std_logic_vector(6 downto 0)
		);
	end component;
	
	signal display_temp: std_logic_vector(3 downto 0);
	signal display_out: std_logic_vector(6 downto 0);

---------   ClockPrescaler   ---------
	component ClockPrescaler is
		 port(
			  clk_in		: 	in std_logic;
			  scaled_clk	: 	out std_logic
		 );
	end component;

	signal clk_2kHz : std_logic;

---------      Debounce      ---------
	component Debounce is
		generic(
			counter_size: integer := 19); --counter size (19 bits gives 10.5ms with 50MHz clock)
		port(
			clk_in, BTN: in std_logic;
			result: out std_logic
		);
	end component;

	signal BTN0, BTN1, BTN2, BTN3: std_logic;

---------    Extra signals   ---------
	signal Test: std_logic_vector(7 downto 0);
--------------   Main   --------------
begin

	-- Prescaler
	ClockPrescaler0: 	ClockPrescaler port map(
		clk_in => clk,
		scaled_clk => clk_2kHz
	);

	-- Encoder
	Encoder0: 	Encoder 
	generic map(
		EncoderBitWidth => 4
	)
	port map(
		CLK => CLK,
		RST => BTN2,
		Input => memory_storage,
		Output => Encoder_Val(7 downto 4)
	);
	
	-- T_FlipFlop
	DP_Select_T_FlipFlop0: T_FlipFlop port map(input => clk_2kHz,  	output => state(0));
	DP_Select_T_FlipFlop1: T_FlipFlop port map(input => not state(0), output => state(1));

	-- Memory
	Memory0: Memory port map(
		SET => BTN0,	
		RST => BTN2,
		input => SW,
		clk_in => CLK,
		output => memory_storage
	);
	
	-- Display Decoder
	Display_Driver0: Display_Driver port map(input => display_temp, output => display_out);
	
	-- Debounce
	BTN0_Debounce: Debounce port map(clk_in => clk, BTN => BTN(0), result => BTN0);
	BTN1_Debounce: Debounce port map(clk_in => clk, BTN => BTN(1), result => BTN1);
	BTN2_Debounce: Debounce port map(clk_in => clk, BTN => BTN(2), result => BTN2);
	BTN3_Debounce: Debounce port map(clk_in => clk, BTN => BTN(3), result => BTN3); 

	-- Logic
	with state select -- Select Display
		AN <= "1110" when "00",
				"1101" when "01",
				"1011" when "10",
				"0111" when "11",
				"1111" when others;

	with state select -- Select Output for current display
		display_temp <= Encoder_Val(7 downto 4) when "11",
							 Encoder_Val(3 downto 0) when "10",
							 "0000" when "01",
							 "0000" when "00",
							 "0000" when others;

	DP <= display_out;
		
end logic;