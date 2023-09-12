#ifndef _IMG_BRAM_HPP_
#define _IMG_BRAM_HPP_

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <vector>
#include "address.hpp"
#include <iostream>


class Img_Bram : public sc_core::sc_module
{
public:
	Img_Bram (sc_core::sc_module_name name);
	~Img_Bram();
	tlm_utils::simple_target_socket<Img_Bram> ip_core_socket;
	tlm_utils::simple_target_socket<Img_Bram> interconnect_socket;
	
protected:
	void b_transport(pl_t&, sc_core::sc_time&);
	std::vector<unsigned char> img;
};

#endif

