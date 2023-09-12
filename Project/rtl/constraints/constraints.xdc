# Clock definition
create_clock -period 7.0 -name clk [get_ports clk]

# Input delay constraints
set_input_delay -clock clk -max 1.0 [get_ports {rst start width_i height_i treshold_i rho_i acc0_addr_cont_i acc0_data_cont_i acc0_we_cont 
                                                acc1_addr_cont_i acc1_data_cont_i acc1_we_cont img_addr_cont_i img_data_cont_i img_we_cont}]

# Output delay constraints
set_output_delay -clock clk -max 1.0 [get_ports {acc0_data_cont_o acc1_data_cont_o img_data_cont_o ready}]

