#ifndef CPU_HPP_ 
#define CPU_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <systemc>

#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
    
#include <stdint.h>
#include "types.hpp"
#include "address.hpp"
#include "functions.hpp"

using namespace std;

class Cpu : public sc_core::sc_module
{
public:
	Cpu(sc_core::sc_module_name name,char** strings, int argv);
	~Cpu();
	tlm_utils::simple_initiator_socket<Cpu> interconnect_socket;
	
protected:
	static char** data_string;
    	static int argc;
    	
    	pl_t pl;
	sc_core::sc_time offset;
	void cpu();
	
	void copying(unsigned char *img, unsigned char *same_img, size_t img_size);
	void monochromating(unsigned char *img, unsigned char *gray_img, size_t img_size);
	void edging(unsigned char *img, unsigned char *edge_img, int width, int height, size_t img_size);
	
	void acc_init(unsigned char *acc, int rho);
	//void hough(unsigned char *img, unsigned char acc[THETA2][RHO], int width, int height, uint8_t transform_threshold);
	int16_t max_acc(unsigned char *acc0, unsigned char *acc1, int rho);
	void draw_lines(unsigned char *img, unsigned char *acc0, unsigned char *acc1, int rho, int width, int height, int16_t acc_max, dw_t draw_factor);

	void read_bram(sc_dt::uint64 addr, unsigned char *val, int length);
	void write_bram(sc_dt::uint64 addr, unsigned char *val, int length);
	int read_hard(sc_dt::uint64 addr);
	void write_hard(sc_dt::uint64 addr,int val);
};

#endif
