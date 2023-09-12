library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity rom is
    generic (WIDTH: positive := 32;
             SIZE: positive := 180;
			 SIZE_WIDTH: positive := 8);
    port (clk_a : in std_logic;
          clk_b : in std_logic;
          en_a: in std_logic;
          en_b: in std_logic;
          addr_a : in std_logic_vector(SIZE_WIDTH - 1 downto 0);
          addr_b : in std_logic_vector(SIZE_WIDTH - 1 downto 0);
          data_a_o: out std_logic_vector(WIDTH - 1 downto 0);
          data_b_o: out std_logic_vector(WIDTH - 1 downto 0));
end rom;

architecture Behavioral of rom is
    type rom_type is array(SIZE - 1 downto 0) of std_logic_vector(WIDTH - 1 downto 0);
    signal ROM : rom_type := (
                        X"011dc002", X"023bc009", X"0359c016", X"0476c027", 
                        X"0593c03e", X"06b0c059", X"07ccc07a", X"08e8c09f", 
                        X"0a03c0c9", X"0b1dc0f8", X"0c36c12d", X"0d4ec166", 
                        X"0e65c1a3", X"0f7bc1e6", X"1090c22e", X"11a4c27a", 
                        X"12b6c2cb", X"13c6c321", X"14d6c37c", X"15e3c3dc", 
                        X"16efc440", X"17f9c4a9", X"1901c516", X"1a07c588", 
                        X"1b0cc5ff", X"1c0ec67a", X"1d0ec6f9", X"1e0bc77d", 
                        X"1f07c806", X"1fffc893", X"20f6c924", X"21eac9b9", 
                        X"22dbca53", X"23c9caf1", X"24b5cb93", X"259ecc39", 
                        X"2684cce3", X"2766cd91", X"2846ce43", X"2923cef9", 
                        X"29fccfb2", X"2ad3d070", X"2ba5d131", X"2c75d1f6", 
                        X"2d41d2be", X"2e09d38a", X"2eced45a", X"2f8fd52c", 
                        X"304dd603", X"3106d6dc", X"31bcd7b9", X"326ed899", 
                        X"331cd97b", X"33c6da61", X"346cdb4a", X"350edc36", 
                        X"35acdd24", X"3646de15", X"36dbdf09", X"376ce000", 
                        X"37f9e0f8", X"3882e1f4", X"3906e2f1", X"3985e3f1", 
                        X"3a00e4f3", X"3a77e5f8", X"3ae9e6fe", X"3b56e806", 
                        X"3bbfe910", X"3c23ea1c", X"3c83eb29", X"3cdeec39", 
                        X"3d34ed49", X"3d85ee5b", X"3dd1ef6f", X"3e19f084", 
                        X"3e5cf19a", X"3e99f2b1", X"3ed2f3c9", X"3f07f4e2", 
                        X"3f36f5fc", X"3f60f717", X"3f85f833", X"3fa6f94f", 
                        X"3fc1fa6c", X"3fd8fb89", X"3fe9fca6", X"3ff6fdc4", 
                        X"3ffdfee2", X"40000000", X"3ffd011d", X"3ff6023b", 
                        X"3fe90359", X"3fd80476", X"3fc10593", X"3fa606b0", 
                        X"3f8507cc", X"3f6008e8", X"3f360a03", X"3f070b1d", 
                        X"3ed20c36", X"3e990d4e", X"3e5c0e65", X"3e190f7b", 
                        X"3dd11090", X"3d8511a4", X"3d3412b6", X"3cde13c6", 
                        X"3c8314d6", X"3c2315e3", X"3bbf16ef", X"3b5617f9", 
                        X"3ae91901", X"3a771a07", X"3a001b0c", X"39851c0e", 
                        X"39061d0e", X"38821e0b", X"37f91f07", X"376c2000", 
                        X"36db20f6", X"364621ea", X"35ac22db", X"350e23c9", 
                        X"346c24b5", X"33c6259e", X"331c2684", X"326e2766", 
                        X"31bc2846", X"31062923", X"304d29fc", X"2f8f2ad3", 
                        X"2ece2ba5", X"2e092c75", X"2d412d41", X"2c752e09", 
                        X"2ba52ece", X"2ad32f8f", X"29fc304d", X"29233106", 
                        X"284631bc", X"2766326e", X"2684331c", X"259e33c6", 
                        X"24b5346c", X"23c9350e", X"22db35ac", X"21ea3646", 
                        X"20f636db", X"1fff376c", X"1f0737f9", X"1e0b3882", 
                        X"1d0e3906", X"1c0e3985", X"1b0c3a00", X"1a073a77", 
                        X"19013ae9", X"17f93b56", X"16ef3bbf", X"15e33c23", 
                        X"14d63c83", X"13c63cde", X"12b63d34", X"11a43d85", 
                        X"10903dd1", X"0f7b3e19", X"0e653e5c", X"0d4e3e99", 
                        X"0c363ed2", X"0b1d3f07", X"0a033f36", X"08e83f60", 
                        X"07cc3f85", X"06b03fa6", X"05933fc1", X"04763fd8", 
                        X"03593fe9", X"023b3ff6", X"011d3ffd", X"00004000");
    
    attribute ram_style: string;
    attribute ram_style of ROM: signal is "block";
begin
    process(clk_a, clk_b)
    begin
        if (rising_edge(clk_a)) then
            if (en_a = '1') then
                data_a_o <= ROM(to_integer(unsigned(addr_a)));
            end if;
        end if;
        
        if (rising_edge(clk_b)) then
            if (en_b = '1') then
                data_b_o <= ROM(to_integer(unsigned(addr_b)));
            end if;
        end if;
    end process;
end Behavioral;
