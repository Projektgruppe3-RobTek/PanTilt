-- Roundaboutsomething --
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.all;

entity StopWatch is

	port (
			CLK	: 	IN STD_LOGIC;
			JA0	: 	IN STD_LOGIC;
			BTN	: 	IN STD_LOGIC_VECTOR(3 downto 0);
			DP		: 	OUT STD_LOGIC_VECTOR(6 downto 0);
			AN		: 	OUT STD_LOGIC_VECTOR(3 downto 0) -- Used to select a segment on the display
	);
end StopWatch;

architecture logic of StopWatch is

----------    Counter     ----------
	component Counter is
		generic(
			size: integer := 4
		);
		port(
			input, RST: in std_logic;
			MAX: in std_logic_vector(size-1 downto 0);
			output: out std_logic_vector(size-1 downto 0); 
			carry_out: out std_logic
		);
	end component;
	
	type counter_array is array (0 to 5) of std_logic_vector(3 downto 0);
	signal counter_out: counter_array;
	signal counter_carry_out: std_logic_vector(5 downto 0);
	
----------   T_FlipFlop   ----------
	component T_FlipFlop is
		port(
			input	 	: 	in std_logic;
			output 	: 	out std_logic
		);
	end component;
	
	signal state: std_logic_vector(1 downto 0) := "00";
	signal lap_toggle_state, stop_toggle_state: std_logic;

-------------   Memory   -------------	
	component Memory is
		port(
			input: in std_logic_vector(15 downto 0);
			SET, RST, clk_in: in std_logic;
			output: out std_logic_vector(15 downto 0)
		);
	end component;
	
	signal memory_storage: std_logic_vector(31 downto 0);

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
	signal timer_out: std_logic_vector(31 downto 0);
	signal overflow: std_logic;
	signal Memory_Select: std_logic_vector(2 downto 0);
	
--------------   Main   --------------
begin

	-- Prescaler
	ClockPrescaler0: 	ClockPrescaler port map(clk_in => clk, scaled_clk => clk_2kHz);

	-- Counter
	Counter_Time: Counter generic map (size => 32) port map(
		input => CLK, 
		RST => BTN1, 
		MAX => "11111111111111111111111111111111", 
		output => timer_out
	);
	
	Counter_Impulse: Counter generic map (size => 16) port map(
		input => JA0, 
		RST => BTN1,
		MAX => "0000001101111001",		
		--MAX => "01011010", 
		carry_out => overflow
	);

	-- T_FlipFlop
	DP_Select_T_FlipFlop0: T_FlipFlop port map(input => clk_2kHz,  	output => state(0));
	DP_Select_T_FlipFlop1: T_FlipFlop port map(input => not state(0), output => state(1));

	-- Memory
	Memory0: Memory port map(clk_in => CLK, input => timer_out(31 downto 16), RST => BTN2, SET => overflow, output => memory_storage(15 downto 0));
	Memory1: Memory port map(clk_in => CLK, input => timer_out(15 downto 0),  RST => BTN2, SET => overflow, output => memory_storage(31 downto 16));

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

	with Memory_Select select -- Select Output for current display
		display_temp <= memory_storage(15 downto 12) when "110",
							 memory_storage(11 downto 8) 	when "100",
							 memory_storage(7 downto 4) 	when "010",
							 memory_storage(3 downto 0) 	when "000",
							 memory_storage(31 downto 28) when "111",
							 memory_storage(27 downto 24) when "101",
							 memory_storage(23 downto 20) 	when "011",
							 memory_storage(19 downto 16) 	when "001",
							 "0000" when others;

	Memory_Select <= State & BTN3;
	DP <= display_out;
		
end logic;