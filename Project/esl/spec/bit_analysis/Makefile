SRC_CPP=$(wildcard *.cpp)
SRC_HPP=$(wildcard *.hpp)
MAIN_TARGET=main*.cpp
TARGET=$(patsubst %.cpp,%,$(wildcard main*.cpp))
all: $(TARGET)
$(TARGET): $(SRC_CPP) $(SRC_HPP)
	g++ -DSC_INCLUDE_FX -Wall -Werror -I ${SYSTEMC}/include -L${SYSTEMC}/lib-linux64 -lsystemc -o $@ *.cpp
.PHONY: clean
clean:
	rm $(TARGET) 
