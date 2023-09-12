CC = "g++"
PROJECT = cosimulation
SRC = sc_main.cpp ip_core.cpp cpu.cpp interconnect.cpp functions.cpp hough_core.vhd hough_structure.vhd bram.vhd rom.vhd processing_unit.vhd fsm.vhd loop_pipeline_if.vhd dsp_unit_1.vhd dsp_unit_2.vhd dsp_unit_3.vhd dsp_unit_5.vhd dsp_unit_7.vhd dsp_unit_8.vhd

$(PROJECT) : $(SRC)
	xmsc_run -sc_main -DSC_INCLUDE_DYNAMIC_PROCESSES -DSC_INCLUDE_FX +systemc_args+"0.5 ../../data/logo.png 0.6 ../../data/djokovic.png" -gui $(SRC)

	# 0.6 ../../data/"djokovic.png" 0.5 ../../data/"logo.png" 0.5 ../../data/"adidas.png" 0.5 ../../data/"avion.png" 0.4 ../../data/"czvv.png" 0.5 ../../data/"goldengate.png" 0.5 ../../data/"MC.png" 0.5 ../../data/"radijator.png" 0.5 ../../data/"rubic.jpg" 0.5 ../../data/"teren.png" - faktor iscrtavanja se moze podesavati, za nekomentarisane fotografije je vec namesten
	
clean:
	rm result_img*.jpg *.log *.key *.so -r *.d
