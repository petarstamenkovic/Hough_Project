SRC_CPP=$(wildcard *.cpp)
SRC_HPP=$(wildcard *.hpp bit_analysis/img_tr_functions.hpp bit_analysis/stb_image.hpp bit_analysis/stb_image_write.hpp)
MAIN_TARGET=main*.cpp
TARGET=$(patsubst %.cpp,%,$(wildcard main*.cpp))
all: $(TARGET)
$(TARGET): $(SRC_CPP) $(SRC_HPP)
	g++ -Wall -Werror -o $@ *.cpp
.PHONY: clean
clean:
	rm $(TARGET) result_img*.jpg
