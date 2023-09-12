library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use work.utils_pkg.all;

entity processing_unit_tb is
    generic (THETA_WIDTH: natural := 8;
             XY_WIDTH: natural := 10;
             TRIG_WIDTH: natural := 16;
             RHO_WIDTH: natural := 12;
             ADDR_WIDTH: natural := 16;
             DATA_WIDTH: natural := 8;
             ACC_SIZE: natural := 65536);
end processing_unit_tb;

architecture Behavioral of processing_unit_tb is
    signal clk: std_logic;
    signal rst: std_logic;
    signal t2_i, t3_i: std_logic_vector(THETA_WIDTH - 1 downto 0);
    signal x_i: std_logic_vector(XY_WIDTH - 1 downto 0);
    signal y_i: std_logic_vector(XY_WIDTH - 1 downto 0);
    signal sin_i: std_logic_vector(TRIG_WIDTH - 1 downto 0);
    signal cos_i: std_logic_vector(TRIG_WIDTH - 1 downto 0);
    signal rho_i: std_logic_vector(XY_WIDTH - 1 downto 0);
    signal acc_rdata_i: std_logic_vector(DATA_WIDTH - 1 downto 0);
    signal acc_wdata_o: std_logic_vector(DATA_WIDTH - 1 downto 0);
    signal acc_raddr_o: std_logic_vector(log2c(ACC_SIZE) - 1 downto 0);
    signal acc_waddr_o: std_logic_vector(log2c(ACC_SIZE) - 1 downto 0);
    
    component processing_unit is
        generic (THETA_WIDTH: natural := 8;
                 XY_WIDTH: natural := 10;
                 TRIG_WIDTH: natural := 16;
                 RHO_WIDTH: natural := 12;
                 ADDR_WIDTH: natural := 16;
                 DATA_WIDTH: natural := 8;
                 ACC_SIZE: natural := 65536);
        port (clk: in std_logic;
              rst: in std_logic;
              x_i: in std_logic_vector(XY_WIDTH - 1 downto 0);
              y_i: in std_logic_vector(XY_WIDTH - 1 downto 0);
              sin_i: in std_logic_vector(TRIG_WIDTH - 1 downto 0);
              cos_i: in std_logic_vector(TRIG_WIDTH - 1 downto 0);
              t2_i: in std_logic_vector(THETA_WIDTH - 1 downto 0);
              t3_i: in std_logic_vector(THETA_WIDTH - 1 downto 0);
              rho_i: in std_logic_vector(XY_WIDTH - 1 downto 0);
              acc_rdata_i: in std_logic_vector(DATA_WIDTH - 1 downto 0);
              acc_wdata_o: out std_logic_vector(DATA_WIDTH - 1 downto 0);
              acc_raddr_o: out std_logic_vector(log2c(ACC_SIZE) - 1 downto 0);
              acc_waddr_o: out std_logic_vector(log2c(ACC_SIZE) - 1 downto 0));
    end component;
begin
    duv: processing_unit
    generic map(THETA_WIDTH => THETA_WIDTH,
                XY_WIDTH => XY_WIDTH,
                TRIG_WIDTH => TRIG_WIDTH,
                RHO_WIDTH => RHO_WIDTH,
                ADDR_WIDTH => ADDR_WIDTH,
                DATA_WIDTH => DATA_WIDTH,
                ACC_SIZE => ACC_SIZE)
    port map(clk => clk,
             rst => rst,
             x_i => x_i,
             y_i => y_i,
             sin_i => sin_i,
             cos_i => cos_i,
             t2_i => t2_i,
             t3_i => t3_i,
             rho_i => rho_i,
             acc_rdata_i => acc_rdata_i,
             acc_wdata_o => acc_wdata_o,
             acc_raddr_o => acc_raddr_o,
             acc_waddr_o => acc_waddr_o);
               
    clk_gen: process
    begin
        clk <= '0', '1' after 100ns;
        wait for 200ns;
    end process;
    
    stim_gen: process
    begin
        rst <= '0';
        t2_i <= std_logic_vector(to_unsigned(0, THETA_WIDTH));
        t3_i <= std_logic_vector(to_unsigned(0, THETA_WIDTH));
        x_i <= std_logic_vector(to_unsigned(0, XY_WIDTH));
        y_i <= std_logic_vector(to_unsigned(0, XY_WIDTH));
        rho_i <= std_logic_vector(to_unsigned(100, XY_WIDTH));
        acc_rdata_i <= std_logic_vector(to_unsigned(137, DATA_WIDTH));
        sin_i <= X"0000";
        cos_i <= X"0000";
        wait until falling_edge(clk);
        rst <= '1';
        wait until falling_edge(clk);
        rst <= '0';
        x_i <= std_logic_vector(to_unsigned(25, XY_WIDTH));
        y_i <= std_logic_vector(to_unsigned(4, XY_WIDTH));
        sin_i <= X"0000";
        cos_i <= X"4000";
        wait until falling_edge(clk);
        sin_i <= X"023b";
        cos_i <= X"3ff6";
        wait until falling_edge(clk);
        t2_i <= std_logic_vector(to_unsigned(0, THETA_WIDTH));
        sin_i <= X"0476";
        cos_i <= X"3fd8";
        wait until falling_edge(clk);
        t2_i <= std_logic_vector(to_unsigned(1, THETA_WIDTH));
        t3_i <= std_logic_vector(to_unsigned(0, THETA_WIDTH));
        sin_i <= X"06b0";
        cos_i <= X"3fa6";
        wait until falling_edge(clk);
        t2_i <= std_logic_vector(to_unsigned(2, THETA_WIDTH));
        t3_i <= std_logic_vector(to_unsigned(1, THETA_WIDTH));
        sin_i <= X"08e8";
        cos_i <= X"3f60";
        wait until falling_edge(clk);
        t2_i <= std_logic_vector(to_unsigned(3, THETA_WIDTH));
        t3_i <= std_logic_vector(to_unsigned(2, THETA_WIDTH));
        sin_i <= X"0b1d";
        cos_i <= X"3f07";
        wait until falling_edge(clk);
        acc_rdata_i <= std_logic_vector(to_unsigned(255, DATA_WIDTH));
        t2_i <= std_logic_vector(to_unsigned(4, THETA_WIDTH));
        t3_i <= std_logic_vector(to_unsigned(3, THETA_WIDTH));
        sin_i <= X"0d4e";
        cos_i <= X"3e99";
        wait until falling_edge(clk);
        t2_i <= std_logic_vector(to_unsigned(5, THETA_WIDTH));
        t3_i <= std_logic_vector(to_unsigned(4, THETA_WIDTH));
        sin_i <= X"0f7b";
        cos_i <= X"3e19";
        wait until falling_edge(clk);
        t2_i <= std_logic_vector(to_unsigned(6, THETA_WIDTH));
        t3_i <= std_logic_vector(to_unsigned(5, THETA_WIDTH));
        sin_i <= X"11a4";
        cos_i <= X"3d85";
        wait until falling_edge(clk);
        t2_i <= std_logic_vector(to_unsigned(7, THETA_WIDTH));
        t3_i <= std_logic_vector(to_unsigned(6, THETA_WIDTH));
        sin_i <= X"13c6";
        cos_i <= X"3cde";
        wait until falling_edge(clk);
        t2_i <= std_logic_vector(to_unsigned(8, THETA_WIDTH));
        t3_i <= std_logic_vector(to_unsigned(7, THETA_WIDTH));
        x_i <= std_logic_vector(to_unsigned(20, XY_WIDTH));
        y_i <= std_logic_vector(to_unsigned(45, XY_WIDTH));
        sin_i <= X"2684";
        cos_i <= X"cce3";
        wait until falling_edge(clk);
        t2_i <= std_logic_vector(to_unsigned(9, THETA_WIDTH));
        t3_i <= std_logic_vector(to_unsigned(8, THETA_WIDTH));
        wait until falling_edge(clk);
        t2_i <= std_logic_vector(to_unsigned(71, THETA_WIDTH));
        t3_i <= std_logic_vector(to_unsigned(9, THETA_WIDTH));
        wait until falling_edge(clk);
        t3_i <= std_logic_vector(to_unsigned(71, THETA_WIDTH));
        wait;
    end process;
end Behavioral;
