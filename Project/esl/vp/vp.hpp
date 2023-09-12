#ifndef _VP_HPP_
#define _VP_HPP_

#include <systemc>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include "cpu.hpp"
#include "interconnect.hpp"
#include "ip_core.hpp"
#include "sinrom.hpp"
#include "cosrom.hpp"
#include "img_bram.hpp"
#include "acc0_bram.hpp"
#include "acc1_bram.hpp"

class Vp :  public sc_core::sc_module
{
	public:
		Vp(sc_core::sc_module_name name,char** strings, int argv);
		~Vp();

	protected:
		Cpu cpu;
		Interconnect interconnect;
		Ip_Core ip_core;
		CosRom cos_rom;
		SinRom sin_rom;
		Img_Bram img_bram;
		Acc0_Bram acc0_bram;
		Acc1_Bram acc1_bram;
};

#endif 
