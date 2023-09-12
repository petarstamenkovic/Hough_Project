#include "img_bram.hpp"

Img_Bram::Img_Bram(sc_core::sc_module_name name) : sc_module(name)
{
	ip_core_socket.register_b_transport(this, &Img_Bram::b_transport);
	interconnect_socket.register_b_transport(this, &Img_Bram::b_transport);
	img.reserve(IMG_BRAM_SIZE);

	SC_REPORT_INFO("IMG BRAM", "Constructed.");
}

Img_Bram::~Img_Bram()
{
	SC_REPORT_INFO("IMG BRAM", "Destroyed.");
}

void Img_Bram::b_transport(pl_t &pl, sc_core::sc_time &offset)
{
	tlm::tlm_command cmd = pl.get_command();
	sc_dt::uint64 addr = pl.get_address();
	unsigned int len = pl.get_data_length();
	unsigned char *buf = pl.get_data_ptr(); 
	
	switch(cmd)
	{
		case tlm::TLM_WRITE_COMMAND:
			img[addr] =buf[3];
	  		//std::cout << "upis u img_bram: ";
			//std::cout << "adresa: " << (int)(addr) << ", vrednost: " << (int)img[addr] << std::endl;

			pl.set_response_status(tlm::TLM_OK_RESPONSE);
			
			offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
			break;
	
		case tlm::TLM_READ_COMMAND:
			buf[0] = 0;
			buf[1] = 0;
			buf[2] = 0;
			buf[3] = img[addr];
	  		//std::cout << "citanje iz img_bram-a: ";
			//std::cout << "adresa: " << (int)(addr) << ", vrednost: " << (int)img[addr] << std::endl;
			
			pl.set_response_status(tlm::TLM_OK_RESPONSE);
			
			offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
			break;
	
		default:
			pl.set_response_status( tlm::TLM_COMMAND_ERROR_RESPONSE );
			offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
	}
}
