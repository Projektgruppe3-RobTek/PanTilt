------------
--  main  --
------------
library ieee;
use ieee.std_logic_1164.all;

entity main is
	port(
		CLK	:	in  std_logic;				-- FPGA Clock
		
		BTN	:	in  std_logic_vector(1 downto 0);	-- Reset
		--SW	:	in  std_logic_vector(7 downto 0);	-- Test Input
		LD	:	out std_logic_vector(7 downto 0);	-- Test Output
		
		-- SPI0
		JC1	:	in  std_logic;	-- SPI0CLK
		JC2	:	in  std_logic;	-- SPI0SS
		JC3	:	in  std_logic;	-- SPI0MOSI
		JC4	:	out std_logic;	-- SPI0MISO
		
		JD1	:	out std_logic;	-- RW0
		
		JC7	:	in  std_logic;	  -- SPI1CLK
		JC8	:	in  std_logic;	  -- SPI1SS
		JC9	:	in  std_logic;	  -- SPI1MOSI
		JC10	:	out std_logic;	-- SPI1MISO
		
		JD2	:	out std_logic	-- RW1
	);
end main;

architecture logic of main is
	
	component SPIController is
		generic(
			constant PISOBitWidth	:	positive := 8;	-- Size of the parallel input vector
			constant SIPOBitWidth	:	positive := 8	-- Size of the parallel output vector
		);
		port(
			RST		:	in  std_logic;
			CLK		:	in  std_logic;
			
			-- SPI signals
			SPICLK		:	in  std_logic;
			SPISS		:	in  std_logic;
			SPIMOSI		:	in  std_logic;
			SPIMISO		:	out std_logic;
			
			-- Read/Write signal
			RW		:	out std_logic;
			
			-- Parallel input
			PI		: 	in  std_logic_vector(SIPOBitWidth-1 downto 0);
			
			-- Parallel output
			PO		:	out std_logic_vector(SIPOBitWidth-1 downto 0)
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
begin

	COUNTER_32_BIT : COUNTER_X_BIT 
	generic map(SIZE => 32)
  port map( D => CLK,
            RST => '0',
            MAX => "11111111111111111111111111111111",
            Q => CLOCKS
            );
	 

  MEMORY_32_BIT : MEMORY_X_BIT 
  generic map(SIZE => 32)
  port map( in_val => CLOCKS,
           reset => '0',
           out_val =>  saved_clock,
           write => BTN(1),
           clk => CLK
           );
           
	spi1_out <= saved_clock(31 downto 16);
	
	spi0_out <= saved_clock(15 downto 0);
	
	LD <= spi0_in(7 downto 0);
	 
	SPIController0: SPIController
	generic map(
		SIPOBitWidth => 16,
		PISOBitWidth => 16
	)
	port map(
		RST => BTN(0),
		CLK => CLK,
		
		SPICLK => JC1,
		SPISS => JC2,
		SPIMOSI => JC3,
		SPIMISO => JC4,
		
		RW => JD1,
		
		PI => spi0_out,
		PO => spi0_in
	);
	
		SPIController1: SPIController
	generic map(
		SIPOBitWidth => 16,
		PISOBitWidth => 16
	)
	port map(
		RST => BTN(0),
		CLK => CLK,
		
		SPICLK => JC7,
		SPISS => JC8,
		SPIMOSI => JC9,
		SPIMISO => JC10,
		
		RW => JD2,
		
		PI => spi1_out,
		PO => spi1_in
	);
	
end logic;
