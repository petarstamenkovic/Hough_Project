#ifndef _ACC1_BRAM_HPP_
#define _ACC1_BRAM_HPP_

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <vector>
#include "address.hpp"
#include <iostream>


class Acc1_Bram : public sc_core::sc_module
{
public:
	Acc1_Bram (sc_core::sc_module_name name);
	~Acc1_Bram();
	tlm_utils::simple_target_socket<Acc1_Bram> ip_core_socket;
	tlm_utils::simple_target_socket<Acc1_Bram> interconnect_socket;
	
protected:
	void b_transport(pl_t&, sc_core::sc_time&);
	std::vector<unsigned char> acc1;
};

#endif

