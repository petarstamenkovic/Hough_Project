#include "ip_core.hpp"
#include <tlm>

using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm;

SC_HAS_PROCESS(Ip_Core);

Ip_Core::Ip_Core(sc_module_name name):
	sc_module(name),
	core("core"),
	clk("clk", DELAY, sc_core::SC_NS)
{
	interconnect_socket.register_b_transport(this, &Ip_Core::b_transport);

	core.clk( clk.signal() );
	core.reset( reset );
	core.start( start );
	core.width_i( width_i );
	core.height_i( height_i );
	core.treshold_i( treshold_i );
	core.rho_i( rho_i );
	core.acc0_addr_cont_i( acc0_addr_cont_i );
	core.acc0_data_cont_o( acc0_data_cont_o );
	core.acc0_data_cont_i( acc0_data_cont_i );
	core.acc0_we_cont( acc0_we_cont );
	core.acc1_addr_cont_i( acc1_addr_cont_i );
	core.acc1_data_cont_o( acc1_data_cont_o );
	core.acc1_data_cont_i( acc1_data_cont_i );
	core.acc1_we_cont( acc1_we_cont );
	core.img_addr_cont_i( img_addr_cont_i );
	core.img_data_cont_o( img_data_cont_o );
	core.img_data_cont_i( img_data_cont_i );
	core.img_we_cont( img_we_cont );
	core.ready( ready );

	start.write(static_cast<sc_logic> (0));
	reset.write(static_cast<sc_logic> (0));
	width_i.write(static_cast< sc_lv<10> > (0));
	height_i.write(static_cast< sc_lv<10> > (0));
	rho_i.write(static_cast< sc_lv<10> > (0));
	acc0_addr_cont_i.write(static_cast< sc_lv<18> > (0));
	acc0_data_cont_i.write(static_cast< sc_lv<32> > (0));
	acc0_we_cont.write(static_cast< sc_lv<4> > (0));
	acc1_addr_cont_i.write(static_cast< sc_lv<18> > (0));
	acc1_data_cont_i.write(static_cast< sc_lv<32> > (0));
	acc1_we_cont.write(static_cast< sc_lv<4> > (0));
	img_addr_cont_i.write(static_cast< sc_lv<18> > (0));
	img_data_cont_i.write(static_cast< sc_lv<32> > (0));
	img_we_cont.write(static_cast< sc_lv<4> > (0));
	
	SC_REPORT_INFO("Ip_Core", "Constructed.");
}

Ip_Core::~Ip_Core()
{
	SC_REPORT_INFO("Ip_Core", "Destroyed.");
}


void Ip_Core::b_transport(pl_t &pl, sc_core::sc_time &offset)
{
	//cout <<" package recieved" << endl;
	
	tlm::tlm_command cmd = pl.get_command();
 	sc_dt::uint64 addr = pl.get_address();
	sc_dt::uint64 taddr;
	unsigned int len = pl.get_data_length();
 	unsigned char *buf = pl.get_data_ptr();
 	pl.set_response_status( tlm::TLM_OK_RESPONSE );
 	
	switch(cmd)
 	{	
	 	case tlm::TLM_WRITE_COMMAND:
			if (addr == VP_ADDR_IP_HARD_L + ADDR_START)
			{
				start.write(static_cast<sc_logic> (toInt(buf)));
				printf("write %d in start\n", toInt(buf));
				wait(DELAY, SC_NS);
			}
			else if (addr == VP_ADDR_IP_HARD_L + ADDR_RESET)
			{
				reset.write(static_cast<sc_logic> (toInt(buf)));
				printf("write %d in reset\n", toInt(buf));
				wait(DELAY, SC_NS);
			}
			else if (addr == VP_ADDR_IP_HARD_L + ADDR_WIDTH)
			{
				width_i.write(static_cast< sc_lv<10> > (toInt(buf)));
				printf("write %d in width\n", toInt(buf));
				wait(DELAY, SC_NS);
			}
			else if (addr == VP_ADDR_IP_HARD_L + ADDR_HEIGHT)
			{
				height_i.write(static_cast< sc_lv<10> > (toInt(buf)));
				printf("write %d in height\n", toInt(buf));
				wait(DELAY, SC_NS);
			}
			else if (addr == VP_ADDR_IP_HARD_L + ADDR_RHO)
			{
				rho_i.write(static_cast< sc_lv<10> > (toInt(buf)));
				printf("write %d in rho\n", toInt(buf));
				wait(DELAY, SC_NS);
			}
			else if (addr == VP_ADDR_IP_HARD_L + ADDR_THRESHOLD)
			{
				treshold_i.write(static_cast< sc_lv<8> > (toInt(buf)));
				printf("write %d in threshold\n", toInt(buf));
				wait(DELAY, SC_NS);
			}
			else if (addr >= VP_ADDR_ACC0_BRAM_L and addr <= VP_ADDR_ACC0_BRAM_H)
			{
				taddr = addr & 0x000FFFFF;
				acc0_addr_cont_i.write(static_cast< sc_lv<18> > (taddr));
				acc0_data_cont_i.write(static_cast< sc_lv<32> > (toInt(buf)));
				acc0_we_cont.write(static_cast< sc_lv<4> > (15));
				wait(DELAY, SC_NS);
				acc0_we_cont.write(static_cast< sc_lv<4> > (0));
				//printf("write %d in addres %d of acc0 bram\n", toInt(buf), taddr);
			}
			else if (addr >= VP_ADDR_ACC1_BRAM_L and addr <= VP_ADDR_ACC1_BRAM_H)
			{
				taddr = addr & 0x000FFFFF;
				acc1_addr_cont_i.write(static_cast< sc_lv<18> > (taddr));
				acc1_data_cont_i.write(static_cast< sc_lv<32> > (toInt(buf)));
				acc1_we_cont.write(static_cast< sc_lv<4> > (15));
				wait(DELAY, SC_NS);
				acc1_we_cont.write(static_cast< sc_lv<4> > (0));
				//printf("write %d in addres %d of acc1 bram\n", toInt(buf), taddr);
			}
			else if (addr >= VP_ADDR_IMG_BRAM_L and addr <= VP_ADDR_IMG_BRAM_H)
			{
				taddr = addr & 0x000FFFFF;
				img_addr_cont_i.write(static_cast< sc_lv<19> > (taddr));
				img_data_cont_i.write(static_cast< sc_lv<32> > (toInt(buf)));
				img_we_cont.write(static_cast< sc_lv<4> > (15));
				wait(DELAY, SC_NS);
				img_we_cont.write(static_cast< sc_lv<4> > (0));
				//printf("write %d in addres %d of img bram\n", toInt(buf), taddr);
			}
			else
			{
				pl.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
				cout << "Wrong write address!" << endl;
			}
			break;
		case tlm::TLM_READ_COMMAND:
			if (addr == VP_ADDR_IP_HARD_L + ADDR_READY)
			{
				wait(DELAY, SC_NS);
				if (ready.read() == '1')
				{
					toUchar4(buf, 1);
					//printf("read %d from ready\n", toInt(buf));
				}				
				else
				{
					toUchar4(buf, 0);
					//printf("read %d from ready\n", toInt(buf));
				}			

				//toUchar4(buf, (int) (static_cast< sc_lv<32> > (ready.read())));
			}
			else if (addr >= VP_ADDR_ACC0_BRAM_L and addr <= VP_ADDR_ACC0_BRAM_H)
			{
				taddr = addr & 0x000FFFFF;
				acc0_addr_cont_i.write(static_cast< sc_lv<18> > (taddr));
				wait(DELAY, SC_NS);
				toUchar4(buf, static_cast< sc_uint<32> > (acc0_data_cont_o.read()));
				//printf("read %d from addres %d of acc0 bram\n", toInt(buf), taddr);
			}
			else if (addr >= VP_ADDR_ACC1_BRAM_L and addr <= VP_ADDR_ACC1_BRAM_H)
			{
				taddr = addr & 0x000FFFFF;
				acc1_addr_cont_i.write(static_cast< sc_lv<18> > (taddr));
				wait(DELAY, SC_NS);
				toUchar4(buf, static_cast< sc_uint<32> > (acc1_data_cont_o.read()));
				//printf("read %d from addres %d of acc1 bram\n", toInt(buf), taddr);
			}
			else if (addr >= VP_ADDR_IMG_BRAM_L and addr <= VP_ADDR_IMG_BRAM_H)
			{
				taddr = addr & 0x000FFFFF;
				img_addr_cont_i.write(static_cast< sc_lv<19> > (taddr));
				wait(DELAY, SC_NS);
				toUchar4(buf, static_cast< sc_uint<32> > (img_data_cont_o.read()));
				//printf("write %d from addres %d of img bram\n", toInt(buf), taddr);
			}
			else
			{
				pl.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
				cout << "Wrong read address!" << endl;
			}
			break;
		default:
			pl.set_response_status( tlm::TLM_COMMAND_ERROR_RESPONSE );
			cout << "Wrong command!" << endl;
	}
	
	//offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
}


