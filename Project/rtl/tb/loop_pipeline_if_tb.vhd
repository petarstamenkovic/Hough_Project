library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use work.utils_pkg.all;

entity loop_pipeline_if_tb is
    generic (THETA_WIDTH: natural := 8;
             ROM_SIZE: natural := 180);
end loop_pipeline_if_tb;

architecture Behavioral of loop_pipeline_if_tb is
    signal clk: std_logic;
    signal rst: std_logic;
    signal theta_i: std_logic_vector(THETA_WIDTH - 1 downto 0 );
    signal trig0_addr_o: std_logic_vector(log2c(ROM_SIZE) - 1 downto 0);
    signal trig1_addr_o: std_logic_vector(log2c(ROM_SIZE) - 1 downto 0);
    signal t2_o: std_logic_vector(THETA_WIDTH - 1 downto 0);
    signal t3_o: std_logic_vector(THETA_WIDTH - 1 downto 0);
    signal t6_o: std_logic_vector(THETA_WIDTH - 1 downto 0);
    
    component loop_pipeline_if is
        generic (THETA_WIDTH: natural := 8;
                 ROM_SIZE: natural := 180);
        port (clk: in std_logic;
              rst: in std_logic;
              theta_i: in std_logic_vector(THETA_WIDTH - 1 downto 0 );
              trig0_addr_o: out std_logic_vector(log2c(ROM_SIZE) - 1 downto 0);
              trig1_addr_o: out std_logic_vector(log2c(ROM_SIZE) - 1 downto 0);
              t2_o: out std_logic_vector(THETA_WIDTH - 1 downto 0);
              t3_o: out std_logic_vector(THETA_WIDTH - 1 downto 0);
              t6_o: out std_logic_vector(THETA_WIDTH - 1 downto 0));
    end component;
    
begin
    duv: loop_pipeline_if
    generic map(THETA_WIDTH => THETA_WIDTH,
                ROM_SIZE => ROM_SIZE)
    port map(clk => clk,
             rst => rst,
             theta_i => theta_i,
             trig0_addr_o => trig0_addr_o,
             trig1_addr_o => trig1_addr_o,
             t2_o => t2_o,
             t3_o => t3_o,
             t6_o => t6_o);
               
    clk_gen: process
    begin
        clk <= '0', '1' after 100ns;
        wait for 200ns;
    end process;
    
    stim_gen: process
    begin
        rst <= '0';
        wait until falling_edge(clk);
        rst <= '1';
        wait until falling_edge(clk);
        rst <= '0';
        theta_i <= std_logic_vector(to_unsigned(0, THETA_WIDTH));
        wait until falling_edge(clk);
        theta_i <= std_logic_vector(to_unsigned(1, THETA_WIDTH));
        wait until falling_edge(clk);
        theta_i <= std_logic_vector(to_unsigned(2, THETA_WIDTH));
        wait until falling_edge(clk);
        theta_i <= std_logic_vector(to_unsigned(3, THETA_WIDTH));
        wait until falling_edge(clk);
        theta_i <= std_logic_vector(to_unsigned(4, THETA_WIDTH));
        wait until falling_edge(clk);
        theta_i <= std_logic_vector(to_unsigned(5, THETA_WIDTH));
        wait until falling_edge(clk);
        theta_i <= std_logic_vector(to_unsigned(6, THETA_WIDTH));
        wait until falling_edge(clk);
        theta_i <= std_logic_vector(to_unsigned(7, THETA_WIDTH));
        wait until falling_edge(clk);
        theta_i <= std_logic_vector(to_unsigned(8, THETA_WIDTH));
        wait until falling_edge(clk);
        theta_i <= std_logic_vector(to_unsigned(9, THETA_WIDTH));
        wait until falling_edge(clk);
        theta_i <= std_logic_vector(to_unsigned(10, THETA_WIDTH));
        wait until falling_edge(clk);
        theta_i <= std_logic_vector(to_unsigned(11, THETA_WIDTH));
        wait until falling_edge(clk);
        wait;
    end process;
end Behavioral;