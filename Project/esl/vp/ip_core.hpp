#ifndef _IP_HARD_H_
#define _IP_HARD_H_

#ifndef SC_INCLUDE_FX
#define SC_INCLUDE_FX
#endif

#include <systemc>
#include <sysc/datatypes/fx/sc_fixed.h>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#include "address.hpp"
#include "functions.hpp"
#include "types.hpp"

using namespace std;

class Ip_Core :
	public sc_core::sc_module
{
public:
	Ip_Core(sc_core::sc_module_name);
	~Ip_Core();
	
	tlm_utils::simple_target_socket<Ip_Core> interconnect_socket;
	tlm_utils::simple_initiator_socket<Ip_Core> sin_rom_socket;
	tlm_utils::simple_initiator_socket<Ip_Core> cos_rom_socket;
	tlm_utils::simple_initiator_socket<Ip_Core> img_bram_socket;
	tlm_utils::simple_initiator_socket<Ip_Core> acc0_bram_socket;
	tlm_utils::simple_initiator_socket<Ip_Core> acc1_bram_socket;

protected:
	pl_t pl;
	sc_core::sc_time offset;
	
	//registri
	sc_dt::sc_uint<1> start_reg;
	sc_dt::sc_uint<1> reset_reg;
	sc_dt::sc_uint<10> width_reg;
	sc_dt::sc_uint<10> height_reg;
	sc_dt::sc_uint<10> rho_reg;
	sc_dt::sc_uint<8> threshold_reg;
	sc_dt::sc_uint<1> ready_reg;
 
	void b_transport(pl_t&, sc_core::sc_time&);	
	void hough(sc_core::sc_time &);
	
	unsigned char read_bram(sc_dt::sc_uint<64> addr);
	void write_bram(sc_dt::sc_uint<64> addr, sc_dt::sc_uint <8> val);
	
	trig_t read_rom(sc_dt::sc_uint<64> addr);
};

#endif
