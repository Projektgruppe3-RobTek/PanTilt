-- Roundaboutsomething --
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.all;

entity StopWatch is

	port (
			CLK	: 	IN STD_LOGIC;
			JA9	: 	IN STD_LOGIC;
			BTN	: 	IN STD_LOGIC_VECTOR(3 downto 0);
			DP		: 	OUT STD_LOGIC_VECTOR(6 downto 0);
			AN		: 	OUT STD_LOGIC_VECTOR(3 downto 0); -- Used to select a segment on the display
			SW : IN STD_LOGIC_VECTOR(7 downto 0);
			JB8 : OUT STD_LOGIC; --Enable A
			JB3 : OUT STD_LOGIC; -- IN1A
			JB9 : OUT STD_LOGIC;  -- IN2A
			LD : OUT STD_LOGIC_VECTOR(0 downto 0)
			
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
		overflow_val: in std_logic_vector(size-1 downto 0);
		output: out std_logic_vector(size-1 downto 0); 
		carry_out: out std_logic;
		overflow: out std_logic
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
	signal memory_storage1: std_logic_vector(31 downto 0);

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
		    prescaler: in std_logic_vector (14 downto 0) ;
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
	
	component SignedPWM is
	
	generic(
		-- define bit width of pwm
		constant SignedBitWidth: positive := 8
	);
	
	port(
		-- clock impulse to count
		CounterClock: in std_logic;
		-- compare match input vector
		CompareMatch: in std_logic_vector((SignedBitWidth - 1) downto 0);
		-- pwm output
		Output: out std_logic_vector(1 downto 0)
	);
	
	end component;

	signal BTN0, BTN1, BTN2, BTN3: std_logic;

---------    Extra signals   ---------
	signal timer_out: std_logic_vector(31 downto 0);
	signal overflow: std_logic ;
	signal Memory_Select: std_logic_vector(2 downto 0);
	signal clk_20kHz : std_logic;
	signal pwm_out : std_logic_vector(1 downto 0);
	signal stop : std_logic;

	
--------------   Main   --------------
begin


	LD(0) <= stop;
	-- Memory
	Memory0: Memory port map(clk_in => CLK, input => timer_out(31 downto 16), RST => BTN2, SET => stop, output => memory_storage(31 downto 16));
	Memory1: Memory port map(clk_in => CLK, input => timer_out(15 downto 0),  RST => BTN2, SET => stop, output => memory_storage(15 downto 0));

		
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
		display_temp <= memory_storage1(15 downto 12) when "111",
							 memory_storage(11 downto 8) 	when "101",
							 memory_storage(7 downto 4) 	when "011",
							 memory_storage(3 downto 0) 	when "001",
							 memory_storage(31 downto 28) when "110",
							 memory_storage(27 downto 24) when "100",
							 memory_storage(23 downto 20) 	when "010",
							 memory_storage(19 downto 16) 	when "000",
							 "0000" when others;

	Memory_Select <= State & BTN3;
	DP <= display_out;
	
	Counter_Time: Counter generic map (size => 32) port map(
		input => CLK, 
		RST => overflow, 
		MAX => 					"11111111111111111111111111111111",
		overflow_val => 	"00000000000010110101011000000001",
		overflow => stop
	);
	
	Counter_Time1: Counter generic map (size => 32) port map(
		input => CLK, 
		RST => BTN2, 
		MAX => 					"11111111111111111111111111111111",
		overflow_val => "00000000000000000000000000000000",
		output => timer_out
	);
	
	Counter_Impulse: Counter generic map (size => 4) port map(
		input => JA9, 
		RST => BTN1,
		MAX => "0010",
		overflow_val => "0000",	
		carry_out => overflow
	);
	
	PMW0 : SignedPWM port map(CounterClock => clk_20kHz, CompareMatch => SW, Output => pwm_out );
	JB3 <= pwm_out(0);
	JB9 <= pwm_out(1);
	JB8 <= '1';
		
		
			-- Prescaler
	ClockPrescaler0: 	ClockPrescaler port map(clk_in => clk, scaled_clk => clk_2kHz, prescaler => "110000110101000");
	ClockPrescaler1: 	ClockPrescaler port map(clk_in => clk, scaled_clk => clk_20kHz, prescaler => "000001011100010"); --20Khz
	
		-- T_FlipFlop
	DP_Select_T_FlipFlop0: T_FlipFlop port map(input => clk_2kHz,  	output => state(0));
	DP_Select_T_FlipFlop1: T_FlipFlop port map(input => not state(0), output => state(1));

end logic;
