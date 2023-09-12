library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use work.utils_pkg.all;

entity hough_structure_tb is
    generic (THETA_WIDTH: natural := 8;
             XY_WIDTH: natural := 10;
             TRIG_WIDTH: natural := 16;
             RHO_WIDTH: natural := 12;
             ADDR_WIDTH: natural := 16;
             DATA_WIDTH: natural := 8;
             ACC_SIZE: natural := 65536;
             ROM_SIZE: natural := 180);
end hough_structure_tb;

architecture Behavioral of hough_structure_tb is
    signal clk: std_logic;
    signal rst: std_logic;
    signal theta_i: std_logic_vector(THETA_WIDTH - 1 downto 0);
    signal x_i: std_logic_vector(XY_WIDTH - 1 downto 0);
    signal y_i: std_logic_vector(XY_WIDTH - 1 downto 0);
    signal sin_i: std_logic_vector(TRIG_WIDTH - 1 downto 0);
    signal cos_i: std_logic_vector(TRIG_WIDTH - 1 downto 0);
    signal rho_i: std_logic_vector(XY_WIDTH - 1 downto 0);
    signal acc_rdata_i: std_logic_vector(DATA_WIDTH - 1 downto 0);
    signal acc_wdata_o: std_logic_vector(DATA_WIDTH - 1 downto 0);
    signal acc_raddr_o: std_logic_vector(log2c(ACC_SIZE) - 1 downto 0);
    signal acc_waddr_o: std_logic_vector(log2c(ACC_SIZE) - 1 downto 0);
    signal trig0_addr_o: std_logic_vector(log2c(ROM_SIZE) - 1 downto 0);
    signal trig1_addr_o: std_logic_vector(log2c(ROM_SIZE) - 1 downto 0);
    signal t_status_o: std_logic_vector(THETA_WIDTH - 1 downto 0);
    
    component hough_structure is
        generic (THETA_WIDTH: natural := 8;
                 XY_WIDTH: natural := 10;
                 TRIG_WIDTH: natural := 16;
                 RHO_WIDTH: natural := 12;
                 ADDR_WIDTH: natural := 16;
                 DATA_WIDTH: natural := 8;
                 ACC_SIZE: natural := 65536;
                 ROM_SIZE: natural := 180);
        port (clk: in std_logic;
              rst: in std_logic;
              theta_i: in std_logic_vector(THETA_WIDTH - 1 downto 0);
              x_i: in std_logic_vector(XY_WIDTH - 1 downto 0);
              y_i: in std_logic_vector(XY_WIDTH - 1 downto 0);
              sin_i: in std_logic_vector(TRIG_WIDTH - 1 downto 0);
              cos_i: in std_logic_vector(TRIG_WIDTH - 1 downto 0);
              rho_i: in std_logic_vector(XY_WIDTH - 1 downto 0);
              acc_rdata_i: in std_logic_vector(DATA_WIDTH - 1 downto 0);
              acc_wdata_o: out std_logic_vector(DATA_WIDTH - 1 downto 0);
              acc_raddr_o: out std_logic_vector(log2c(ACC_SIZE) - 1 downto 0);
              acc_waddr_o: out std_logic_vector(log2c(ACC_SIZE) - 1 downto 0);
              trig0_addr_o: out std_logic_vector(log2c(ROM_SIZE) - 1 downto 0);
              trig1_addr_o: out std_logic_vector(log2c(ROM_SIZE) - 1 downto 0);
              t_status_o: out std_logic_vector(THETA_WIDTH - 1 downto 0));
    end component;
begin
    duv: hough_structure
    generic map(THETA_WIDTH => THETA_WIDTH,
                XY_WIDTH => XY_WIDTH,
                TRIG_WIDTH => TRIG_WIDTH,
                RHO_WIDTH => RHO_WIDTH,
                ADDR_WIDTH => ADDR_WIDTH,
                DATA_WIDTH => DATA_WIDTH,
                ACC_SIZE => ACC_SIZE,
                ROM_SIZE => ROM_SIZE)
    port map(clk => clk,
             rst => rst,
             theta_i => theta_i,
             x_i => x_i,
             y_i => y_i,
             sin_i => sin_i,
             cos_i => cos_i,
             rho_i => rho_i,
             acc_rdata_i => acc_rdata_i,
             acc_wdata_o => acc_wdata_o,
             acc_raddr_o => acc_raddr_o,
             acc_waddr_o => acc_waddr_o,
             trig0_addr_o => trig0_addr_o,
             trig1_addr_o => trig1_addr_o,
             t_status_o => t_status_o);
    
    clk_gen: process
    begin
        clk <= '0', '1' after 100ns;
        wait for 200ns;
    end process;
    
    stim_gen: process
    begin
        rst <= '1';
        x_i <= std_logic_vector(to_unsigned(0, XY_WIDTH));
        y_i <= std_logic_vector(to_unsigned(0, XY_WIDTH));
        sin_i <= X"0000";
        cos_i <= X"0000";
        wait until falling_edge(clk);
        rst <= '0';
        rho_i <= std_logic_vector(to_unsigned(100, XY_WIDTH));
        theta_i <= std_logic_vector(to_unsigned(71, THETA_WIDTH));
        acc_rdata_i <= std_logic_vector(to_unsigned(137, DATA_WIDTH));
        wait until falling_edge(clk);
        wait until falling_edge(clk);
        x_i <= std_logic_vector(to_unsigned(20, XY_WIDTH));
        y_i <= std_logic_vector(to_unsigned(45, XY_WIDTH));
        sin_i <= X"2684";
        cos_i <= X"cce3";
        wait;
    end process;
end Behavioral;
