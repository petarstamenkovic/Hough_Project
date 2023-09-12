----------------------------------------------------------------------------------
-- Company: Katedra za elektroniku, DEET, FTN, UNS
-- Engineer: Dejan Pejic
-- 
-- Create Date: 06/23/2023 02:58:02 PM
-- Design Name: 
-- Module Name: hough_core_tb - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity hough_core_tb is
    generic (THETA_WIDTH: natural := 8;
             XY_WIDTH: natural := 10;
             TRIG_WIDTH: natural := 16;
             RHO_WIDTH: natural := 12;
             DIMENSIONS_WIDTH: natural := 10;
             TRESHOLD_WIDTH: natural := 8;
             ADDR_WIDTH: natural := 16;
             DATA_WIDTH: natural := 8;
             BUS_DATA_WIDTH: natural := 32;
             ROM_DATA_WIDTH: natural := 32;
             IMG_SIZE: natural := 98304;
             ACC_SIZE: natural := 60000;
             ROM_SIZE: natural := 180;
             ADDR_OFFSET: natural := 2;
             ADDR_FACTOR: natural := 4;
			 IMG_SIZE_WIDTH: natural := 17;
			 ACC_SIZE_WIDTH: natural := 16;
			 ROM_SIZE_WIDTH: natural := 8);
end hough_core_tb;

architecture Behavioral of hough_core_tb is
    signal clk: std_logic;
    signal reset: std_logic;
    signal axi_reset: std_logic;
    signal start: std_logic;
    signal width_i: std_logic_vector(DIMENSIONS_WIDTH - 1 downto 0);
    signal height_i: std_logic_vector(DIMENSIONS_WIDTH - 1 downto 0);
    signal treshold_i: std_logic_vector(TRESHOLD_WIDTH - 1 downto 0);
    signal rho_i: std_logic_vector(XY_WIDTH - 1 downto 0);
    signal acc0_addr_cont_i: std_logic_vector(ACC_SIZE_WIDTH + ADDR_OFFSET - 1 downto 0);
    signal acc0_data_cont_o: std_logic_vector(BUS_DATA_WIDTH - 1 downto 0);
    signal acc0_data_cont_i: std_logic_vector(BUS_DATA_WIDTH - 1 downto 0);
    signal acc0_we_cont: std_logic_vector(3 downto 0);
    signal acc1_addr_cont_i: std_logic_vector(ACC_SIZE_WIDTH + ADDR_OFFSET - 1 downto 0);
    signal acc1_data_cont_o: std_logic_vector(BUS_DATA_WIDTH - 1 downto 0);
    signal acc1_data_cont_i: std_logic_vector(BUS_DATA_WIDTH - 1 downto 0);
    signal acc1_we_cont: std_logic_vector(3 downto 0);
    signal img_addr_cont_i: std_logic_vector(IMG_SIZE_WIDTH + ADDR_OFFSET - 1 downto 0);
    signal img_data_cont_o: std_logic_vector(BUS_DATA_WIDTH - 1 downto 0);
    signal img_data_cont_i: std_logic_vector(BUS_DATA_WIDTH - 1 downto 0);
    signal img_we_cont: std_logic_vector(3 downto 0);
    signal ready: std_logic;
    
    constant width_c: integer := 12;
    constant height_c: integer := 10; 
    constant treshold_c: integer := 140;
    constant rho_c: integer := 15;
    constant half_theta_c: integer := 135;
    
    component hough_core is
        generic (THETA_WIDTH: natural := 8;
                 XY_WIDTH: natural := 10;
                 TRIG_WIDTH: natural := 16;
                 RHO_WIDTH: natural := 12;
                 DIMENSIONS_WIDTH: natural := 10;
                 TRESHOLD_WIDTH: natural := 8;
                 ADDR_WIDTH: natural := 16;
                 DATA_WIDTH: natural := 8;
                 BUS_DATA_WIDTH: natural := 32;
                 ROM_DATA_WIDTH: natural := 32;
                 IMG_SIZE: natural := 98304;
                 ACC_SIZE: natural := 60000;
                 ROM_SIZE: natural := 180;
                 ADDR_OFFSET: natural := 2;
			     IMG_SIZE_WIDTH: natural := 17;
			     ACC_SIZE_WIDTH: natural := 16;
			     ROM_SIZE_WIDTH: natural := 8);
        port (clk: in std_logic;
              reset: in std_logic;
              axi_reset: in std_logic;
              start: in std_logic;
              width_i: in std_logic_vector(DIMENSIONS_WIDTH - 1 downto 0);
              height_i: in std_logic_vector(DIMENSIONS_WIDTH - 1 downto 0);
              treshold_i: in std_logic_vector(TRESHOLD_WIDTH - 1 downto 0);
              rho_i: in std_logic_vector(XY_WIDTH - 1 downto 0);
              acc0_addr_cont_i: in std_logic_vector(ACC_SIZE_WIDTH + ADDR_OFFSET - 1 downto 0);
              acc0_data_cont_o: out std_logic_vector(BUS_DATA_WIDTH - 1 downto 0);
              acc0_data_cont_i: in std_logic_vector(BUS_DATA_WIDTH - 1 downto 0);
              acc0_we_cont: in std_logic_vector(3 downto 0);
              acc1_addr_cont_i: in std_logic_vector(ACC_SIZE_WIDTH + ADDR_OFFSET - 1 downto 0);
              acc1_data_cont_o: out std_logic_vector(BUS_DATA_WIDTH - 1 downto 0);
              acc1_data_cont_i: in std_logic_vector(BUS_DATA_WIDTH - 1 downto 0);
              acc1_we_cont: in std_logic_vector(3 downto 0);
              img_addr_cont_i: in std_logic_vector(IMG_SIZE_WIDTH + ADDR_OFFSET - 1 downto 0);
              img_data_cont_o: out std_logic_vector(BUS_DATA_WIDTH - 1 downto 0);
              img_data_cont_i: in std_logic_vector(BUS_DATA_WIDTH - 1 downto 0);
              img_we_cont: in std_logic_vector(3 downto 0);
              ready: out std_logic);
    end component;
begin
    duv: hough_core 
    generic map (THETA_WIDTH => THETA_WIDTH,
                 XY_WIDTH => XY_WIDTH,
                 TRIG_WIDTH => TRIG_WIDTH,
                 RHO_WIDTH => RHO_WIDTH,
                 DIMENSIONS_WIDTH => DIMENSIONS_WIDTH,
                 TRESHOLD_WIDTH => TRESHOLD_WIDTH,
                 ADDR_WIDTH => ADDR_WIDTH,
                 DATA_WIDTH => DATA_WIDTH,
                 ROM_DATA_WIDTH => ROM_DATA_WIDTH,
                 IMG_SIZE => IMG_SIZE,
                 ACC_SIZE => ACC_SIZE,
                 ROM_SIZE => ROM_SIZE)
    port map (clk => clk,
              reset => reset,
              axi_reset => axi_reset,
              start => start,
              width_i => width_i,
              height_i => height_i,
              treshold_i => treshold_i,
              rho_i => rho_i,
              acc0_addr_cont_i => acc0_addr_cont_i,
              acc0_data_cont_o => acc0_data_cont_o,
              acc0_data_cont_i => acc0_data_cont_i,
              acc0_we_cont => acc0_we_cont,
              acc1_addr_cont_i => acc1_addr_cont_i,
              acc1_data_cont_o => acc1_data_cont_o,
              acc1_data_cont_i => acc1_data_cont_i,
              acc1_we_cont => acc1_we_cont,
              img_addr_cont_i => img_addr_cont_i,
              img_data_cont_o => img_data_cont_o,
              img_data_cont_i => img_data_cont_i,
              img_we_cont => img_we_cont,
              ready => ready);

    clk_gen: process
    begin
        clk <= '0', '1' after 5ns;
        wait for 10ns;
    end process;
    
    stim_gen: process
    begin
        axi_reset <= '1';
        reset <= '1';
        start <= '0';
        width_i <= std_logic_vector(to_unsigned(width_c, DIMENSIONS_WIDTH));
        height_i <= std_logic_vector(to_unsigned(height_c, DIMENSIONS_WIDTH));
        treshold_i <= std_logic_vector(to_unsigned(140, TRESHOLD_WIDTH));
        rho_i <= std_logic_vector(to_unsigned(rho_c, XY_WIDTH));
        wait until falling_edge(clk);
        reset <= '0';
        
        acc0_we_cont <= "1111";
        for i in 0 to rho_c * half_theta_c - 1 loop
            acc0_addr_cont_i <= std_logic_vector(to_unsigned(ADDR_FACTOR * i, ACC_SIZE_WIDTH + ADDR_OFFSET));
            acc0_data_cont_i <= (others => '0');
            wait until falling_edge(clk);
        end loop;
        acc0_we_cont <= "0000";
        
        acc1_we_cont <= "1111";
        for i in 0 to rho_c * half_theta_c - 1 loop
            acc1_addr_cont_i <= std_logic_vector(to_unsigned(ADDR_FACTOR * i, ACC_SIZE_WIDTH + ADDR_OFFSET));
            acc1_data_cont_i <= (others => '0');
            wait until falling_edge(clk);
        end loop;
        acc1_we_cont <= "0000";
        
        img_we_cont <= "1111";
        
        for i in 0 to width_c * height_c  - 1 loop
            img_addr_cont_i <= std_logic_vector(to_unsigned(ADDR_FACTOR * i, IMG_SIZE_WIDTH + ADDR_OFFSET));
            img_data_cont_i <= (others => '0');
            wait until falling_edge(clk);
        end loop;
        img_addr_cont_i <= std_logic_vector(to_unsigned(ADDR_FACTOR * (0 * width_c + 0), IMG_SIZE_WIDTH + ADDR_OFFSET));
        img_data_cont_i <= std_logic_vector(to_unsigned(199, BUS_DATA_WIDTH));
        wait until falling_edge(clk);
        img_addr_cont_i <= std_logic_vector(to_unsigned(ADDR_FACTOR * (2 * width_c + 2), IMG_SIZE_WIDTH + ADDR_OFFSET));
        img_data_cont_i <= std_logic_vector(to_unsigned(199, BUS_DATA_WIDTH));
        wait until falling_edge(clk);
        img_addr_cont_i <= std_logic_vector(to_unsigned(ADDR_FACTOR * (4 * width_c + 4), IMG_SIZE_WIDTH + ADDR_OFFSET));
        img_data_cont_i <= std_logic_vector(to_unsigned(199, BUS_DATA_WIDTH));
        wait until falling_edge(clk);
        img_addr_cont_i <= std_logic_vector(to_unsigned(ADDR_FACTOR * (7 * width_c + 7), IMG_SIZE_WIDTH + ADDR_OFFSET));
        img_data_cont_i <= std_logic_vector(to_unsigned(199, BUS_DATA_WIDTH));
        wait until falling_edge(clk);
        img_addr_cont_i <= std_logic_vector(to_unsigned(ADDR_FACTOR * (3 * width_c + 11), IMG_SIZE_WIDTH + ADDR_OFFSET));
        img_data_cont_i <= std_logic_vector(to_unsigned(153, BUS_DATA_WIDTH));
        wait until falling_edge(clk);
        img_addr_cont_i <= std_logic_vector(to_unsigned(ADDR_FACTOR * (9 * width_c + 5), IMG_SIZE_WIDTH + ADDR_OFFSET));
        img_data_cont_i <= std_logic_vector(to_unsigned(153, BUS_DATA_WIDTH));
        wait until falling_edge(clk);
        img_we_cont <= "0000";
        
        wait until falling_edge(clk);
        start <= '1';
        wait until falling_edge(clk);
        wait until falling_edge(clk);
        start <= '0';
        wait until ready = '1';
        
        for i in 0 to rho_c * half_theta_c - 1 loop
            acc0_addr_cont_i <= std_logic_vector(to_unsigned(ADDR_FACTOR * i, ACC_SIZE_WIDTH + ADDR_OFFSET));
            wait until falling_edge(clk);
        end loop;
        
        for i in 0 to rho_c * half_theta_c - 1 loop
            acc1_addr_cont_i <= std_logic_vector(to_unsigned(ADDR_FACTOR * i, ACC_SIZE_WIDTH + ADDR_OFFSET));
            wait until falling_edge(clk);
        end loop;
        
        acc0_addr_cont_i <= std_logic_vector(to_unsigned(ADDR_FACTOR * (30 * rho_c + 2), ACC_SIZE_WIDTH + ADDR_OFFSET));
        acc1_addr_cont_i <= std_logic_vector(to_unsigned(ADDR_FACTOR * (67 * rho_c + 9), ACC_SIZE_WIDTH + ADDR_OFFSET));
        wait until falling_edge(clk);
        
        acc0_addr_cont_i <= std_logic_vector(to_unsigned(ADDR_FACTOR * (105 * rho_c + 3), ACC_SIZE_WIDTH + ADDR_OFFSET));
        acc1_addr_cont_i <= std_logic_vector(to_unsigned(ADDR_FACTOR * (22 * rho_c + 0), ACC_SIZE_WIDTH + ADDR_OFFSET));
        wait until falling_edge(clk);
        wait;
    end process;
end Behavioral;
