#include "acc0_bram.hpp"

Acc0_Bram::Acc0_Bram(sc_core::sc_module_name name) : sc_module(name)
{
	ip_core_socket.register_b_transport(this, &Acc0_Bram::b_transport);
	interconnect_socket.register_b_transport(this, &Acc0_Bram::b_transport);
	acc0.reserve(ACC_BRAM_SIZE);
	
	for(int i = 0; i != ACC_BRAM_SIZE; ++i)
		acc0.push_back(0);

	SC_REPORT_INFO("ACC BRAM", "Constructed.");
}

Acc0_Bram::~Acc0_Bram()
{
	SC_REPORT_INFO("ACC0 BRAM", "Destroyed.");
}

void Acc0_Bram::b_transport(pl_t &pl, sc_core::sc_time &offset)
{
	tlm::tlm_command cmd = pl.get_command();
	sc_dt::uint64 addr = pl.get_address();
	unsigned int len = pl.get_data_length();
	unsigned char *buf = pl.get_data_ptr(); 
	
	switch(cmd)
	{
		case tlm::TLM_WRITE_COMMAND:
	  		acc0[addr] =buf[3];
	  		//std::cout << "upis u acc0_bram: ";
			//std::cout << "adresa: " << (int)(addr) << ", vrednost: " << (int)acc0[addr] << std::endl;

			pl.set_response_status(tlm::TLM_OK_RESPONSE);
			
			offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
			break;
	
		case tlm::TLM_READ_COMMAND:
			buf[0] = 0;
			buf[1] = 0;
			buf[2] = 0;
	  		buf[3] = acc0[addr];
	  		//std::cout << "citanje iz acc0_bram-a: ";
			//std::cout << "adresa: " << (int)(addr) << ", vrednost: " << (int)acc0[addr] << std::endl;
			
			pl.set_response_status(tlm::TLM_OK_RESPONSE);
			
			offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
			break;
	
		default:
			pl.set_response_status( tlm::TLM_COMMAND_ERROR_RESPONSE );
			offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
	}
}
