#ifndef _COSROM_HPP_
#define _COSROM_HPP_

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <vector>
#include "address.hpp"
#include "types.hpp"
#include <iostream>

class CosRom : public sc_core::sc_module
{
public:
	CosRom (sc_core::sc_module_name name);
	~CosRom();
	tlm_utils::simple_target_socket<CosRom> ip_core_socket;
	
protected:
	void b_transport(pl_t&, sc_core::sc_time&);
	std::vector<trig_t> cosrom;
};

#endif
