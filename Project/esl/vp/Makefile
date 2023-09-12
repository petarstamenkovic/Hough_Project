CC = "g++"
PROJECT = hough
SRC = main.cpp functions.cpp cpu.cpp interconnect.cpp ip_core.cpp sinrom.cpp cosrom.cpp img_bram.cpp acc0_bram.cpp acc1_bram.cpp vp.cpp

$(PROJECT) : $(SRC)
	$(CC) -DSC_INCLUDE_FX -I ${SYSTEMC}/include -L${SYSTEMC}/lib-linux64 -lsystemc $(SRC) -o $(PROJECT)
run:  
	./$(PROJECT) 0.5 ../../data/"avion.png" #1 ../../data/"tb_img.png" 0.5 ../../data/adidas.png #0.6 ../../data/"djokovic.png" #0.5 ../../data/"logo.png" 1 ../../data/"tb_img.png" 0.5 ../../data/"avion.png" 0.4 ../../data/"czvv.png" 0.5 ../../data/"goldengate.png" 0.5 ../../data/"MC.png" 0.5 ../../data/"radijator.png" 0.5 ../../data/"rubic.jpg" 0.5 ../../data/"teren.png" - faktor iscrtavanja se moze podesavati, za nekomentarisane fotografije je vec namesten
	
clean:
	rm hough result_img*.jpg *.txt
