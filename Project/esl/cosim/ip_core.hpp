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
#include "hough_core.hpp"

using namespace std;

class Ip_Core :
	public sc_core::sc_module
{
public:
	Ip_Core(sc_core::sc_module_name);
	~Ip_Core();
	
	tlm_utils::simple_target_socket<Ip_Core> interconnect_socket;

protected:
	pl_t pl;

	sc_core::sc_time offset;

	hough_core core;
	sc_core::sc_clock clk;
	sc_core::sc_signal< sc_dt::sc_logic > reset;
	sc_core::sc_signal< sc_dt::sc_logic > start;
	sc_core::sc_signal< sc_dt::sc_lv<10> > width_i;
	sc_core::sc_signal< sc_dt::sc_lv<10> > height_i;
	sc_core::sc_signal< sc_dt::sc_lv<8> > treshold_i;
	sc_core::sc_signal< sc_dt::sc_lv<10> > rho_i;
	sc_core::sc_signal< sc_dt::sc_lv<18> > acc0_addr_cont_i;
	sc_core::sc_signal< sc_dt::sc_lv<32> > acc0_data_cont_o;
	sc_core::sc_signal< sc_dt::sc_lv<32> > acc0_data_cont_i;
	sc_core::sc_signal< sc_dt::sc_lv<4> > acc0_we_cont;
	sc_core::sc_signal< sc_dt::sc_lv<18> > acc1_addr_cont_i;
	sc_core::sc_signal< sc_dt::sc_lv<32> > acc1_data_cont_o;
	sc_core::sc_signal< sc_dt::sc_lv<32> > acc1_data_cont_i;
	sc_core::sc_signal< sc_dt::sc_lv<4> > acc1_we_cont;
	sc_core::sc_signal< sc_dt::sc_lv<19> > img_addr_cont_i;
	sc_core::sc_signal< sc_dt::sc_lv<32> > img_data_cont_o;
	sc_core::sc_signal< sc_dt::sc_lv<32> > img_data_cont_i;
	sc_core::sc_signal< sc_dt::sc_lv<4> > img_we_cont;
	sc_core::sc_signal< sc_dt::sc_logic > ready;
 
	void b_transport(pl_t&, sc_core::sc_time&);
};

#ifndef SC_MAIN
SC_MODULE_EXPORT(Ip_Core)
#endif

#endif
