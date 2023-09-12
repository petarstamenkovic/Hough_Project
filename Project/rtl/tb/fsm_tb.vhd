library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use work.utils_pkg.all;

entity fsm_tb is
    generic (XY_WIDTH: natural := 10;
             DIMENSIONS_WIDTH: natural := 10;
             TRESHOLD_WIDTH: natural := 8;
             THETA_WIDTH: natural := 8;
             DATA_WIDTH: natural := 8;
             IMG_SIZE: natural := 108000);
end fsm_tb;

architecture Behavioral of fsm_tb is
    signal clk: std_logic;
    signal rst: std_logic;
    signal start: std_logic;
    signal theta_status_i: std_logic_vector(THETA_WIDTH - 1 downto 0);
    signal width_i: std_logic_vector(DIMENSIONS_WIDTH - 1 downto 0);
    signal height_i: std_logic_vector(DIMENSIONS_WIDTH - 1 downto 0);
    signal treshold_i: std_logic_vector(TRESHOLD_WIDTH - 1 downto 0);
    signal img_data_i: std_logic_vector(DATA_WIDTH - 1 downto 0);
    signal img_addr_o: std_logic_vector(log2c(IMG_SIZE) - 1 downto 0);
    signal x_o: std_logic_vector(XY_WIDTH - 1 downto 0);
    signal y_o: std_logic_vector(XY_WIDTH - 1 downto 0);
    signal theta_o: std_logic_vector(THETA_WIDTH - 1 downto 0);
    signal rom_en_o: std_logic;
    signal img_en_o: std_logic;
    signal acc0_we_o: std_logic_vector(3 downto 0);
    signal acc1_we_o: std_logic_vector(3 downto 0);
    signal ready: std_logic;
    
    component fsm is
        generic (XY_WIDTH: natural := 10;
                 DIMENSIONS_WIDTH: natural := 10;
                 TRESHOLD_WIDTH: natural := 8;
                 THETA_WIDTH: natural := 8;
                 DATA_WIDTH: natural := 8;
                 IMG_SIZE: natural := 108000);
        port (clk: in std_logic;
              rst: in std_logic;
              start: in std_logic;
              theta_status_i: in std_logic_vector(THETA_WIDTH - 1 downto 0);
              width_i: in std_logic_vector(DIMENSIONS_WIDTH - 1 downto 0);
              height_i: in std_logic_vector(DIMENSIONS_WIDTH - 1 downto 0);
              treshold_i: in std_logic_vector(TRESHOLD_WIDTH - 1 downto 0);
              img_data_i: in std_logic_vector(DATA_WIDTH - 1 downto 0);
              img_addr_o: out std_logic_vector(log2c(IMG_SIZE) - 1 downto 0);
              x_o: out std_logic_vector(XY_WIDTH - 1 downto 0);
              y_o: out std_logic_vector(XY_WIDTH - 1 downto 0);
              theta_o: out std_logic_vector(THETA_WIDTH - 1 downto 0);
              rom_en_o: out std_logic;
              img_en_o: out std_logic;
              acc0_we_o: out std_logic_vector(3 downto 0);
              acc1_we_o: out std_logic_vector(3 downto 0);
              ready: out std_logic);
    end component;
begin
    duv: fsm
    generic map(XY_WIDTH => XY_WIDTH,
                DIMENSIONS_WIDTH => DIMENSIONS_WIDTH,
                TRESHOLD_WIDTH => TRESHOLD_WIDTH,
                THETA_WIDTH => THETA_WIDTH,
                DATA_WIDTH => DATA_WIDTH,
                IMG_SIZE => IMG_SIZE)
    port map(clk => clk,
             rst => rst,
             start => start,
             theta_status_i => theta_status_i,
             width_i => width_i,
             height_i => height_i,
             treshold_i => treshold_i,
             img_data_i => img_data_i,
             img_addr_o => img_addr_o,
             x_o => x_o,
             y_o => y_o,
             theta_o => theta_o,
             rom_en_o => rom_en_o,
             img_en_o => img_en_o,
             acc0_we_o => acc0_we_o,
             acc1_we_o => acc1_we_o,
             ready => ready);
    
    clk_gen: process
    begin
        clk <= '0', '1' after 4ns;
        wait for 8ns;
    end process;
    
    stim_gen: process
    begin
        rst <= '1';
        start <= '0';
        width_i <= std_logic_vector(to_unsigned(8, DIMENSIONS_WIDTH));
        height_i <= std_logic_vector(to_unsigned(6, DIMENSIONS_WIDTH));
        treshold_i <= std_logic_vector(to_unsigned(140, TRESHOLD_WIDTH));
        theta_status_i <= (others => '0');
        img_data_i <= (others => '0');
        wait until falling_edge(clk);
        rst <= '0';
        wait until falling_edge(clk);
        wait until falling_edge(clk);
        wait until falling_edge(clk);
        start <= '1';
        wait until falling_edge(clk);
        start <= '0';
        wait until img_addr_o = std_logic_vector(to_unsigned(32, log2c(IMG_SIZE)));
        img_data_i <= std_logic_vector(to_unsigned(185, DATA_WIDTH));
        wait until theta_o = std_logic_vector(to_unsigned(96, TRESHOLD_WIDTH));
        theta_status_i <= std_logic_vector(to_unsigned(89, TRESHOLD_WIDTH));
        img_data_i <= (others => '0');
        wait until img_addr_o = std_logic_vector(to_unsigned(44, log2c(IMG_SIZE)));
        theta_status_i <= std_logic_vector(to_unsigned(96, TRESHOLD_WIDTH));
        img_data_i <= std_logic_vector(to_unsigned(141, DATA_WIDTH));
        wait until theta_o = std_logic_vector(to_unsigned(96, TRESHOLD_WIDTH));
        theta_status_i <= std_logic_vector(to_unsigned(89, TRESHOLD_WIDTH));
        img_data_i <= (others => '0');
        wait until ready = '1';
        wait until falling_edge(clk);
        wait until falling_edge(clk);
        wait;
    end process;
end Behavioral;
