library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_arith.all;

entity SPI_Slave is
	generic(
		clk_phase		:	std_logic := '0';
		clk_polarity	:	std_logic := '0';
		data_width		:	integer 	 := 8
	);
	port(
		ss_n	: 	in  std_logic; -- Slave select, active low
		sclk	:	in  std_logic; -- SPI clock
		mosi	:	in  std_logic; -- Master out, Slave in
		miso	: 	out std_logic; -- Master in, Slave out
		
		reset_n			: 	in  std_logic; -- Reset, active low
		tx_load_en		: 	in  std_logic; -- Transmit buffer -> load enable
		tx_load_data	:	in  std_logic_vector(data_width-1 downto 0); -- Transmit buffer -> load data
		rx_req			: 	in  std_logic; -- Request last data from master
		rx_data			:	out std_logic_vector(data_width-1 downto 0) -- Last data received from master
	);
end SPI_Slave;

architecture logic of SPI_Slave is
	signal mode				:	std_logic; -- Mode of the clock
	signal clk				:	std_logic; -- clk of the process
	signal bit_count		: 	std_logic_vector(data_width-1 downto 0);   -- Transaction bits -> '1' = active
	signal rx_buffer		:	std_logic_vector(data_width-1 downto 0); -- Receiver buffer
	signal tx_buffer		:	std_logic_vector(data_width-1 downto 0); -- Transmitter buffer
	
begin
	-- Adjust clock
	mode <= clk_phase xor clk_polarity;
	with mode select
		clk <= 	  sclk when '1',
				 not sclk when others;
	
	-- Keep track of which bit is active
	process(clk, ss_n) begin
		if(ss_n = '1') then -- If slave not selected or being reset
			bit_count(data_width-1) <= '1';
			bit_count(data_width-2 downto 0) <= (others => '0'); -- Reset bit_count
		else
			if(rising_edge(clk)) then -- Slave selected
				bit_count(data_width-2 downto 0) <= bit_count(data_width-1 downto 1); -- Shift register that keep track of which bit is active
				bit_count(data_width-1) <= ss_n;
			end if;
		end if;
	end process;
	
	-- Main process
	process(ss_n, clk, tx_load_en, rx_req) begin
		 
		-- MOSI - Receive
		if(rising_edge(clk)) then
			if(reset_n = '0') then
				rx_buffer <= (others => '0');
			elsif(ss_n = '0') then
				for i in 0 to data_width-1 loop          
					if(bit_count(data_width-1 - i) = '1') then
						rx_buffer(data_width-1 - i) <= mosi;
					end if;
				end loop;
			end if;
		end if;
		 
		-- Request for received data
		if(reset_n = '0') then
			rx_data <= (others => '0');
		elsif(ss_n = '1' and rx_req = '1') then  
			rx_data <= rx_buffer;
		end if;
		 
		-- Load data into transmit buffer
		if(reset_n = '0') then
			tx_buffer <= (others => '0');
		elsif(ss_n = '1' and tx_load_en = '1') then
			tx_buffer <= tx_load_data;
		end if;

		 -- MISO - Transmit
		if(rising_edge(clk)) then
			if(ss_n = '1' or reset_n = '0') then -- No transaction or reset
				miso <= '0';
			else
				for i in 0 to data_width-1 loop          
					if(bit_count(data_width-1 - i) = '1') then
						miso <= tx_buffer(data_width-1 - i); -- Transmit data to master
					end if;
				end loop; 
			end if;
		end if;
	end process;
end logic;

