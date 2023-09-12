# Stvaranje direktorijuma u kojem ce biti smesten projekat
cd ..
file mkdir project_folder
cd project_folder

# Stvaranje projekta
create_project hough_project hough_project -part xc7z010clg400-1
set_property board_part digilentinc.com:zybo-z7-10:part0:1.0 [current_project]
set_property target_language VHDL [current_project]
set_property simulator_language VHDL [current_project]

# Otvaranje novog projekta za pakovanj IP jezgra
create_peripheral xilinx.com user hough 1.0 -dir ip_repo
add_peripheral_interface S00_AXI -interface_mode slave -axi_type lite [ipx::find_open_core xilinx.com:user:hough:1.0]
set_property VALUE 7 [ipx::get_bus_parameters WIZ_NUM_REG -of_objects [ipx::get_bus_interfaces S00_AXI -of_objects [ipx::find_open_core xilinx.com:user:hough:1.0]]]
generate_peripheral -driver -bfm_example_design -debug_hw_example_design [ipx::find_open_core xilinx.com:user:hough:1.0]
write_peripheral [ipx::find_open_core xilinx.com:user:hough:1.0]
set_property  ip_repo_paths  ip_repo/hough_1.0 [current_project]
update_ip_catalog -rebuild
ipx::edit_ip_in_project -upgrade true -name edit_hough_v1_0 -directory ip_repo ip_repo/hough_1.0/component.xml
update_compile_order -fileset sources_1

# Ucitavanje potrebnih fajlova i podesavanje vrha hijerarhije
add_files -norecurse -copy_to ip_repo/hough_1.0/src ../models/rom.vhd
add_files -norecurse -copy_to ip_repo/hough_1.0/src ../models/bram.vhd
add_files -norecurse -copy_to ip_repo/hough_1.0/src {../models/dsp_unit_3.vhd ../models/dsp_unit_2.vhd ../models/dsp_unit_5.vhd ../models/dsp_unit_1.vhd ../models/dsp_unit_7.vhd ../models/dsp_unit_8.vhd}
add_files -norecurse -copy_to ip_repo/hough_1.0/src {../models/hough_core.vhd ../models/hough_structure.vhd ../models/processing_unit.vhd ../models/fsm.vhd ../models/loop_pipeline_if.vhd}
add_files -norecurse -copy_to ip_repo/hough_1.0/src {../models/Hough_IP_v1_0.vhd ../models/Hough_IP_v1_0_S00_AXI.vhd}
update_compile_order -fileset sources_1
set_property top Hough_IP_v1_0 [current_fileset]
update_compile_order -fileset sources_1

# Pokretanje sinteze radi provere ispravnosti IP jezgra
launch_runs synth_1 -jobs 6
wait_on_run synth_1

# Podesavanje parametara IP jezgra
set_property vendor FTN [ipx::current_core]
set_property library y23_g00 [ipx::current_core]
set_property name hough_ip [ipx::current_core]
set_property display_name hough_ip_v1.0 [ipx::current_core]
set_property description {IP Core for the Hough transformation} [ipx::current_core]
set_property supported_families {zynq Pre-Production} [ipx::current_core]
set_property core_revision 1 [ipx::current_core]

# Zavrsna faza pakovanja jezgra i zatvaranje projekta
ipx::merge_project_changes files [ipx::current_core]
ipx::merge_project_changes hdl_parameters [ipx::current_core]
ipx::create_xgui_files [ipx::current_core]
ipx::update_checksums [ipx::current_core]
ipx::save_core [ipx::current_core]
ipx::move_temp_component_back -component [ipx::current_core]
close_project -delete
update_ip_catalog -rebuild -repo_path ip_repo/hough_1.0

# Stvaranje blok-dizajna
create_bd_design "hough_bd"
update_compile_order -fileset sources_1

# Ubacivanje Zynq procesorske jedinice i njena podesavanja
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:processing_system7:5.5 processing_system7_0
endgroup
apply_bd_automation -rule xilinx.com:bd_rule:processing_system7 -config {make_external "FIXED_IO, DDR" apply_board_preset "1" Master "Disable" Slave "Disable" }  [get_bd_cells processing_system7_0]
set_property -dict [list CONFIG.PCW_QSPI_PERIPHERAL_ENABLE {1} CONFIG.PCW_ENET0_PERIPHERAL_ENABLE {1} CONFIG.PCW_SD0_PERIPHERAL_ENABLE {1} CONFIG.PCW_UART1_PERIPHERAL_ENABLE {1} CONFIG.PCW_SPI0_PERIPHERAL_ENABLE {1} CONFIG.PCW_USB0_PERIPHERAL_ENABLE {1} CONFIG.PCW_I2C0_PERIPHERAL_ENABLE {1} CONFIG.PCW_GPIO_MIO_GPIO_ENABLE {1}] [get_bd_cells processing_system7_0]
set_property -dict [list CONFIG.PCW_FPGA0_PERIPHERAL_FREQMHZ {125}] [get_bd_cells processing_system7_0]

# Ubacivanje Hough IP jedinice
startgroup
create_bd_cell -type ip -vlnv FTN:y23_g00:hough_ip:1.0 hough_ip_0
endgroup

# Ubacivanje BRAM jedinica, podesavanje prisustva samo jednog pristupa i omogucivanje burst-moda
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_0
endgroup
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_1
endgroup
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_2
endgroup
set_property -dict [list CONFIG.SINGLE_PORT_BRAM {1}] [get_bd_cells axi_bram_ctrl_0]
set_property -dict [list CONFIG.SUPPORTS_NARROW_BURST.VALUE_SRC USER] [get_bd_cells axi_bram_ctrl_0]
set_property -dict [list CONFIG.SUPPORTS_NARROW_BURST {1}] [get_bd_cells axi_bram_ctrl_0]
set_property -dict [list CONFIG.SINGLE_PORT_BRAM {1}] [get_bd_cells axi_bram_ctrl_1]
set_property -dict [list CONFIG.SUPPORTS_NARROW_BURST.VALUE_SRC USER] [get_bd_cells axi_bram_ctrl_1]
set_property -dict [list CONFIG.SUPPORTS_NARROW_BURST {1}] [get_bd_cells axi_bram_ctrl_1]
set_property -dict [list CONFIG.SINGLE_PORT_BRAM {1}] [get_bd_cells axi_bram_ctrl_2]
set_property -dict [list CONFIG.SUPPORTS_NARROW_BURST.VALUE_SRC USER] [get_bd_cells axi_bram_ctrl_2]
set_property -dict [list CONFIG.SUPPORTS_NARROW_BURST {1}] [get_bd_cells axi_bram_ctrl_2]

# Automatsko povezivanje axi portova
startgroup
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/processing_system7_0/M_AXI_GP0} Slave {/axi_bram_ctrl_0/S_AXI} ddr_seg {Auto} intc_ip {New AXI SmartConnect} master_apm {0}}  [get_bd_intf_pins axi_bram_ctrl_0/S_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/processing_system7_0/M_AXI_GP0} Slave {/axi_bram_ctrl_1/S_AXI} ddr_seg {Auto} intc_ip {New AXI SmartConnect} master_apm {0}}  [get_bd_intf_pins axi_bram_ctrl_1/S_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/processing_system7_0/M_AXI_GP0} Slave {/axi_bram_ctrl_2/S_AXI} ddr_seg {Auto} intc_ip {New AXI SmartConnect} master_apm {0}}  [get_bd_intf_pins axi_bram_ctrl_2/S_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/processing_system7_0/M_AXI_GP0} Slave {/hough_ip_0/S00_AXI} ddr_seg {Auto} intc_ip {New AXI Interconnect} master_apm {0}}  [get_bd_intf_pins hough_ip_0/S00_AXI]
endgroup

# Podesavanje kapaciteta BRAM jedinica i njihovo povezivanje sa Hough IP
set_property range 256K [get_bd_addr_segs {processing_system7_0/Data/SEG_axi_bram_ctrl_0_Mem0}]
set_property range 256K [get_bd_addr_segs {processing_system7_0/Data/SEG_axi_bram_ctrl_1_Mem0}]
set_property range 512K [get_bd_addr_segs {processing_system7_0/Data/SEG_axi_bram_ctrl_2_Mem0}]
connect_bd_net [get_bd_pins axi_bram_ctrl_0/bram_addr_a] [get_bd_pins hough_ip_0/acc0_addr_cont_i]
connect_bd_net [get_bd_pins axi_bram_ctrl_0/bram_wrdata_a] [get_bd_pins hough_ip_0/acc0_data_cont_i]
connect_bd_net [get_bd_pins axi_bram_ctrl_0/bram_rddata_a] [get_bd_pins hough_ip_0/acc0_data_cont_o]
connect_bd_net [get_bd_pins axi_bram_ctrl_0/bram_we_a] [get_bd_pins hough_ip_0/acc0_we_cont]
connect_bd_net [get_bd_pins axi_bram_ctrl_1/bram_addr_a] [get_bd_pins hough_ip_0/acc1_addr_cont_i]
connect_bd_net [get_bd_pins axi_bram_ctrl_1/bram_wrdata_a] [get_bd_pins hough_ip_0/acc1_data_cont_i]
connect_bd_net [get_bd_pins axi_bram_ctrl_1/bram_rddata_a] [get_bd_pins hough_ip_0/acc1_data_cont_o]
connect_bd_net [get_bd_pins axi_bram_ctrl_1/bram_we_a] [get_bd_pins hough_ip_0/acc1_we_cont]
connect_bd_net [get_bd_pins axi_bram_ctrl_2/bram_addr_a] [get_bd_pins hough_ip_0/img_addr_cont_i]
connect_bd_net [get_bd_pins axi_bram_ctrl_2/bram_wrdata_a] [get_bd_pins hough_ip_0/img_data_cont_i]
connect_bd_net [get_bd_pins axi_bram_ctrl_2/bram_rddata_a] [get_bd_pins hough_ip_0/img_data_cont_o]
connect_bd_net [get_bd_pins axi_bram_ctrl_2/bram_we_a] [get_bd_pins hough_ip_0/img_we_cont]
validate_bd_design

# Prostorno uredjivanje blok-dizajna
set_property location {1.5 366 318} [get_bd_cells processing_system7_0]
set_property location {2 690 51} [get_bd_cells axi_smc]
set_property location {2 665 347} [get_bd_cells rst_ps7_0_125M]
set_property location {3 976 609} [get_bd_cells axi_bram_ctrl_0]
set_property location {3 1044 782} [get_bd_cells axi_bram_ctrl_1]
set_property location {3 1134 974} [get_bd_cells axi_bram_ctrl_2]
set_property location {3.5 1513 602} [get_bd_cells hough_ip_0]
regenerate_bd_layout -routing

# Stvaranje HDL wrapper-a
make_wrapper -files [get_files hough_project/hough_project.srcs/sources_1/bd/hough_bd/hough_bd.bd] -top
add_files -norecurse hough_project/hough_project.srcs/sources_1/bd/hough_bd/hdl/hough_bd_wrapper.vhd

# Pokretanje generisanja bitstream-a (uz podrazumevanu sintezu i implementaciju)
launch_runs impl_1 -to_step write_bitstream -jobs 6
wait_on_run impl_1

# Eksportovanje hardvera
write_hw_platform -fixed -force  -include_bit -file hough_bd_wrapper.xsa

# Kopiranje .xsa i .bit fajlova u vitis folder
file copy -force hough_bd_wrapper.xsa ../vitis/hough_bd_wrapper.xsa
file copy -force hough_project/hough_project.runs/impl_1/hough_bd_wrapper.bit ../vitis/hough_bd_wrapper.bit