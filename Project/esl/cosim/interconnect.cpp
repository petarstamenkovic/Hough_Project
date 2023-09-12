#include "interconnect.hpp"

using namespace std;

Interconnect::Interconnect(sc_core::sc_module_name name)
  : sc_module(name)
  , offset(sc_core::SC_ZERO_TIME)
{
  cpu_socket.register_b_transport(this, &Interconnect::b_transport);
  SC_REPORT_INFO("Interconnect", "Constructed.");
}

Interconnect::~Interconnect()
{
  SC_REPORT_INFO("Interconnect", "Destroyed.");
}

void Interconnect::b_transport(pl_t &pl, sc_core::sc_time &offset)
{
  	sc_dt::uint64 addr = pl.get_address();

	if (addr >= VP_ADDR_ACC0_BRAM_L && addr <= VP_ADDR_IMG_BRAM_H)
	{
		pl.set_address(addr);
		ip_core_socket->b_transport(pl, offset);
	}
	
	else
	{
		SC_REPORT_ERROR("Interconnect", "Wrong address.");
		pl.set_response_status ( tlm::TLM_ADDRESS_ERROR_RESPONSE );
		offset += sc_core::sc_time(5 * DELAY, sc_core::SC_NS);
	}
}
