#ifndef _HOUGH_CORE_HPP_
#define _HOUGH_CORE_HPP_

#include <systemc>

class hough_core : public sc_core::sc_foreign_module
{
public:
	hough_core(sc_core::sc_module_name name) :
		sc_core::sc_foreign_module(name),
		clk("clk"),
		reset("reset"),
		start("start"),
		width_i("width_i"),
		height_i("height_i"),
		treshold_i("treshold_i"),
		rho_i("rho_i"),
		acc0_addr_cont_i("acc0_addr_cont_i"),
		acc0_data_cont_o("acc0_data_cont_o"),
		acc0_data_cont_i("acc0_data_cont_i"),
		acc0_we_cont("acc0_we_cont"),
		acc1_addr_cont_i("acc1_addr_cont_i"),
		acc1_data_cont_o("acc1_data_cont_o"),
		acc1_data_cont_i("acc1_data_cont_i"),
		acc1_we_cont("acc1_we_cont"),
		img_addr_cont_i("img_addr_cont_i"),
		img_data_cont_o("img_data_cont_o"),
		img_data_cont_i("img_data_cont_i"),
		img_we_cont("img_we_cont"),
		ready("ready")
	{

	}

	sc_core::sc_in< bool > clk;
	sc_core::sc_in< sc_dt::sc_logic > reset;
	sc_core::sc_in< sc_dt::sc_logic > start;
	sc_core::sc_in< sc_dt::sc_lv<10> > width_i;
	sc_core::sc_in< sc_dt::sc_lv<10> > height_i;
	sc_core::sc_in< sc_dt::sc_lv<8> > treshold_i;
	sc_core::sc_in< sc_dt::sc_lv<10> > rho_i;
	sc_core::sc_in< sc_dt::sc_lv<18> > acc0_addr_cont_i;
	sc_core::sc_out< sc_dt::sc_lv<32> > acc0_data_cont_o;
	sc_core::sc_in< sc_dt::sc_lv<32> > acc0_data_cont_i;
	sc_core::sc_in< sc_dt::sc_lv<4> > acc0_we_cont;
	sc_core::sc_in< sc_dt::sc_lv<18> > acc1_addr_cont_i;
	sc_core::sc_out< sc_dt::sc_lv<32> > acc1_data_cont_o;
	sc_core::sc_in< sc_dt::sc_lv<32> > acc1_data_cont_i;
	sc_core::sc_in< sc_dt::sc_lv<4> > acc1_we_cont;
	sc_core::sc_in< sc_dt::sc_lv<19> > img_addr_cont_i;
	sc_core::sc_out< sc_dt::sc_lv<32> > img_data_cont_o;
	sc_core::sc_in< sc_dt::sc_lv<32> > img_data_cont_i;
	sc_core::sc_in< sc_dt::sc_lv<4> > img_we_cont;
	sc_core::sc_out< sc_dt::sc_logic > ready;

	const char* hdl_name() const { return "hough_core"; }
};

#endif
