------------
--  main  --
------------
library ieee;
use ieee.std_logic_1164.all;

entity main is
	port(
		CLK	:	in  std_logic;				-- FPGA Clock
		
		BTN	:	in  std_logic_vector(0 downto 0);	-- Reset
		
		JD3 : in std_logic;
		JD4 : in std_logic;
		
		-- SPI0
		JC1	:	in  std_logic;	-- SPI0CLK
		JC4	:	out std_logic;	-- SPI0MISO
		
		JC7	:	in  std_logic;	  -- SPI1CLK
		JC10	:	out std_logic	-- SPI1MISO
	);
end main;

architecture logic of main is
	
	component Pulser is
	
	port(
		RST		:	in  std_logic;
		CLK		: 	in  std_logic;
		
		Input		:	in  std_logic;
		Output		:	out std_logic
	);
  end component;

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

component PISO is
	generic(
		constant BitWidth	:	positive := 8	-- Size of the input vector
	);
	port(
		RST	:	in  std_logic;
		
		LATCH	:	in  std_logic;
		CLK	:	in  std_logic;
		
		SI	:	in  std_logic := '0';
		PI	:	in  std_logic_vector(BitWidth-1 downto 0);
		SO	:	out std_logic
	);
end component;
	
	component COUNTER_X_BIT is
	  generic (
		  SIZE:	integer range 0 to 100000 --Size of counter in bits
					  );
					
	  port(
	    D, RST: IN STD_LOGIC;
	    Q: OUT STD_LOGIC_VECTOR(SIZE-1 downto 0);
	    MAX: IN STD_LOGIC_VECTOR(SIZE-1 downto 0)
			    );
	
  end component;
  
  component MEMORY_X_BIT is
	  generic (
		  SIZE:	integer range 0 to 100000 --Size of counter in bits
					  );  
    port (
      in_val   : in std_logic_vector(SIZE-1 downto 0);
      reset : in std_logic;
      out_val  : out std_logic_vector(SIZE-1 downto 0);
      write    : in std_logic;
      clk: in std_logic
      );
    
  end component;

	signal spi0_out: std_logic_vector(15 downto 0);
	signal spi1_out: std_logic_vector(15 downto 0);
	signal spi0_in: std_logic_vector(15 downto 0);
	signal spi1_in: std_logic_vector(15 downto 0);
	signal saved_clock: std_logic_vector(31 downto 0);
	signal CLOCKS: std_logic_vector(31 downto 0);
	signal encoder_in:std_logic_vector(1 downto 0);
	signal encoder_counter: std_logic_vector(7 downto 0);
	signal pulses: std_logic_vector(1 downto 0);
	signal pulse: std_logic;
begin
  pulse <= pulses(0) or pulses(1);

  encoder_in(0) <= JD3;
  encoder_in(1) <= JD4;
  
  Pulser0 : Pulser
  port map(
      RST => BTN(0),
      CLK => CLK,
      Input => encoder_counter(0),
      Output => pulses(0)
  );
   Pulser1 : Pulser
  port map(
      RST => BTN(0),
      CLK => CLK,
      Input => not encoder_counter(0),
      Output => pulses(1)
  );
  
  Encoder0 : Encoder
  generic map(BitWidth => 8)
  port map( RST => BTN(0),
            CLK => CLK,
            Input => encoder_in,
            Output => encoder_counter
            );
  
	COUNTER_32_BIT : COUNTER_X_BIT 
	generic map(SIZE => 32)
  port map( D => CLK,
            RST => BTN(0),
            MAX => "11111111111111111111111111111111",
            Q => CLOCKS
            );
	 

  MEMORY_32_BIT : MEMORY_X_BIT 
  generic map(SIZE => 32)
  port map( in_val => CLOCKS,
           reset => BTN(0),
           out_val =>  saved_clock,
           write => encoder_counter(0),
           clk => CLK
           );
           
	spi1_out <= CLOCKS(31 downto 16);
	
	spi0_out <= CLOCKS(15 downto 0);
	
	
	
	PISO0: PISO
	generic map(
		BitWidth => 16
	)
	port map(
		RST => BTN(0),
		CLK => JC1,
		LATCH => pulse,
		
		PI => spi0_out,
		SO => JC4
	);
	
	PISO1: PISO
	generic map(
		BitWidth => 16
	)
	port map(
		RST => BTN(0),
		CLK => JC7,
		LATCH => pulse,
		
		PI => spi1_out,
		SO => JC10
	);
	
end logic;
