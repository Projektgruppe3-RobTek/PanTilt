-------------------------
--                     --
--    PWMController    --
--                     --
-------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity PWMController is
	generic(
		-- PWM Prescaler value
		constant CLKScale	:	positive := 5;	-- Prescaled Clock = 50MHz / (PWMClockPrescaler * 2)
								-- PWMClockPrescaler -> 50MHz / (5 * 2) = 5MHz;
		-- PWM compare match bit width
		constant BitWidth	:	positive := 8	-- PWM Frequency = 50MHz / (PWMClockPrescaler * 2 * (2 ** PWMBitWidth) - 4)
								-- PWM Frequency -> 50MHz / (5 * 2 * (2 ** 8) - 4) = 20KHz
	);
	port(
		RST		:	in  std_logic;
		CLK		:	in  std_logic;
		
		PWMCM		:	in  std_logic_vector((BitWidth - 1) downto 0);
		
		PWMOutput	:	out std_logic_vector(1 downto 0)
	);
end PWMController;

architecture logic of PWMController is
	
----------   Components   ----------
	
	component PWM is
		generic(
			constant BitWidth	:	positive := 8	-- PWM Frequency = 50MHz / (PWMClockPrescaler * 2 * (2 ** PWMBitWidth) - 4)
		);
		port(
			RST		:	in  std_logic;
			CLK		:	in  std_logic;
			
			CompareMatch	:	in  std_logic_vector(BitWidth-1 downto 0);
			Output		:	out std_logic
		);
	end component;
	
	component Prescaler is
		generic(
			constant Scale	:	positive := 25	-- Prescaled Clock = 50MHz / (PWMClockPrescaler * 2)
		);
		port(
			RST		:	in  std_logic;
			CLK		:	in  std_logic;
			ScaledCLK	:	out std_logic
		);
	end component;
	
----------   Signals   ----------
	
	-- Scaled Clock
	signal sCLK		:	std_logic;
	
	-- Unsigned PWM output
	signal sPWMOut		:	std_logic;
	
	-- Unsigned PWM Compare Match
	signal sComapreMatch	:	std_logic_vector(BitWidth-2 downto 0);
	
begin
	
	-- Invert Compare Match if sined bit is set
	sComapreMatch <=
		PWMCM(BitWidth-2 downto 0) when PWMCM(BitWidth-1) = '0' else
		not PWMCM(BitWidth-2 downto 0);
	
	Prescaler0: Prescaler
	generic map(
		Scale => CLKScale
	)
	port map(
		RST => RST,
		CLK => CLK,
		ScaledCLK => sCLK
	);
	
	PWM0: PWM
	generic map(
		BitWidth => BitWidth-1
	)
	port map(
		RST => RST,
		CLK => sCLK,
		
		CompareMatch => sComapreMatch,
		Output => sPWMOut
	);
	
	-- Connect PWM output according to the signed bit
	PWMOutput(0) <= sPWMOut when PWMCM(BitWidth-1) = '1' else '0';
	PWMOutput(1) <= sPWMOut when PWMCM(BitWidth-1) = '0' else '0';
	
end logic;
