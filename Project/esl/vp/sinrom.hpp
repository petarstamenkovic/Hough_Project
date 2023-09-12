#ifndef _SINROM_HPP_
#define _SINROM_HPP_

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <vector>
#include "address.hpp"
#include "types.hpp"
#include <iostream>

class SinRom : public sc_core::sc_module
{
public:
	SinRom (sc_core::sc_module_name name);
	~SinRom();
	tlm_utils::simple_target_socket<SinRom> ip_core_socket;
	
protected:
	void b_transport(pl_t&, sc_core::sc_time&);
	std::vector<trig_t> sinrom;
};

#endif
