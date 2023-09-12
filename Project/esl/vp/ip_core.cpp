#include "ip_core.hpp"
#include <tlm>

using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm;

SC_HAS_PROCESS(Ip_Core);

Ip_Core::Ip_Core(sc_module_name name):
	sc_module(name),
	ready_reg(1)
{
	interconnect_socket.register_b_transport(this, &Ip_Core::b_transport);
	
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
	unsigned int len = pl.get_data_length();
 	unsigned char *buf = pl.get_data_ptr();
 	pl.set_response_status( tlm::TLM_OK_RESPONSE );
 	
	switch(cmd)
 	{	
	 	case tlm::TLM_WRITE_COMMAND:
	      		switch(addr)
			{
				case ADDR_START:
					start_reg = toInt(buf);
					//cout << "start bit = " << start_reg << endl;
					hough(offset);
					break;
				case ADDR_RESET:
					reset_reg = toInt(buf);
					//cout << "reset bit = " << reset_reg << endl;
					if (reset_reg == 1)
						ready_reg = 1;
					break;
				case ADDR_IMG_WIDTH:
					width_reg = toInt(buf);  
					//cout << "width = " << width_reg << endl;
					break;
				case ADDR_IMG_HEIGHT:
					height_reg = toInt(buf);
					//cout << "height = " << height_reg << endl;
					break;
				case ADDR_RHO:
					rho_reg = toInt(buf);
					//cout << "rho = " << rho_reg << endl;
					break;
				case ADDR_THRESHOLD:
					threshold_reg = toInt(buf);
					//cout << "threshold = " << threshold_reg << endl;
					break;
				default:
					pl.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
					cout << "Pogresna adresa" << endl;
			}
	      		break;
		case tlm::TLM_READ_COMMAND:
			switch(addr)
			{
				case ADDR_READY:
					toUchar4(buf, ready_reg);
					break;
				default:
					pl.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
			}
			break;
		default:
			pl.set_response_status( tlm::TLM_COMMAND_ERROR_RESPONSE );
			cout << "Pogresna komanda" << endl;
	}
	
	offset += sc_core::sc_time(5*DELAY, sc_core::SC_NS);
}

void Ip_Core::hough(sc_core::sc_time &offset)
{
	pl_t pl;
	
	sc_dt::sc_uint <8> theta; 
	sc_dt::sc_int <16> rho;
	
	sc_dt::sc_uint <8> current_pixel;
	
	sc_dt::sc_uint <8> cell_value;
	 
	trig_t sin_pom;
	trig_t cos_pom;
	
	if (start_reg == 1 && ready_reg == 1)  //Ip_core pocinje sa radom
	{
		ready_reg = 0;
		offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
	}
	
	else if (start_reg == 0 && ready_reg == 0)
	{
		//cout << "poocinje" << endl;
;		for (sc_dt::sc_uint<16> y = 0; y < height_reg; y++)
		{
			offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
			//cout << "y = " << y << endl;
			for (sc_dt::sc_uint<16> x = 0; x < width_reg; x++)
			{
				offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
				//cout << "x = " << x << endl;
				current_pixel = read_bram(VP_ADDR_IMG_BRAM_L + y * width_reg + x);
				
				// cout << "ucitao piksel " <<  current_pixel << endl;
				
				if (current_pixel > threshold_reg)
				{
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					//cout << "proverio piksel" << endl;
					for (theta = 0; theta < 180; theta++)
					{
						//cout << "theta = " << theta << endl;
						sin_pom = read_rom(VP_ADDR_SIN_ROM_L + theta);
						cos_pom = read_rom(VP_ADDR_COS_ROM_L + theta);
						rho = x*sin_pom - y*cos_pom;
						offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
						
						//cout << "ucitao sin = " << sin_pom << " i cos = " << cos_pom << ", rho = " << std::dec << rho << endl;
						
						if (rho >= 0)
						{
							if (theta % 2 == 0)
							{
								cell_value = read_bram(VP_ADDR_ACC0_BRAM_L + (theta/2)*rho_reg + rho);
								if (cell_value == 255)
									cell_value = 255;
								else
									cell_value++;
								write_bram(VP_ADDR_ACC0_BRAM_L + (theta/2)*rho_reg + rho, cell_value);
								
								if(((theta/2)*rho_reg + rho) == 25924)
									cout << "25924: x = " << x << ", y = " << y << ", theta = " << theta << ", rho = " << rho << ", sin = " << sin_pom << ", cos = "<< cos_pom << endl;
								if(((theta/2)*rho_reg + rho) == 25925)
									cout << "25925: x = " << x << ", y = " << y << ", theta = " << theta << ", rho = " << rho << ", sin = " << sin_pom << ", cos = "<< cos_pom << endl; 
							}
							else
							{
								cell_value = read_bram(VP_ADDR_ACC1_BRAM_L + ((theta-1)/2)*rho_reg + rho);
								if (cell_value == 255)
									cell_value = 255;
								else
									cell_value++;
								write_bram(VP_ADDR_ACC1_BRAM_L + ((theta-1)/2)*rho_reg + rho, cell_value);
							}
							//cout << "acc[" << theta << "][" << rho << "] = " << cell_value << endl;
						}
						else
						{
							if (theta % 2 == 0)
							{
								cell_value = read_bram(VP_ADDR_ACC0_BRAM_L + ((theta/2) + THETA_HALF)*rho_reg - rho);
								if (cell_value == 255)
									cell_value = 255;
								else
									cell_value++;
								write_bram(VP_ADDR_ACC0_BRAM_L + ((theta/2) + THETA_HALF)*rho_reg - rho, cell_value);
								/*if((((theta/2) + THETA_HALF)*rho_reg - rho) == 46502)
									cout << "46502: x = " << x << ", y = " << y << ", theta = " << theta << ", rho = " << rho << ", sin = " << sin_pom << ", cos = "<< cos_pom << endl;
								if((((theta/2) + THETA_HALF)*rho_reg - rho) == 46503)
									cout << "46503: x = " << x << ", y = " << y << ", theta = " << theta << ", rho = " << rho << ", sin = " << sin_pom << ", cos = "<< cos_pom << endl; */
							}
							else
							{
								cell_value = read_bram(VP_ADDR_ACC1_BRAM_L + (((theta-1)/2) + THETA_HALF)*rho_reg - rho);
								if (cell_value == 255)
									cell_value = 255;
								else
									cell_value++;
								write_bram(VP_ADDR_ACC1_BRAM_L + (((theta-1)/2) + THETA_HALF)*rho_reg - rho, cell_value);
							}
							//cout << "acc[" << THETA + theta << "][" << -rho << "] = " << cell_value << endl;
						}
						offset += sc_core::sc_time(2*DELAY, sc_core::SC_NS);
						//cout << x << " " << y << " " << theta << " " << rho << endl;	    
					}
				}
			}
		}
		
		ready_reg = 1;
	}
}	

void Ip_Core::write_bram(sc_dt::sc_uint<64> addr, sc_dt::sc_uint <8> val)
{
	pl_t pl;
	unsigned char buf[4] = {0, 0, 0, 0};
	buf[3] = to_U(val);
	sc_dt::uint64 taddr = addr & 0x00FFFFFF;
	pl.set_address(taddr);
	pl.set_data_length(BUS_WIDTH); 
	pl.set_data_ptr(buf);
	pl.set_command(tlm::TLM_WRITE_COMMAND);
	pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );
	if (addr >= VP_ADDR_IMG_BRAM_L && addr <= VP_ADDR_IMG_BRAM_H)
	{
		img_bram_socket->b_transport(pl, offset);
	}
	else if (addr >= VP_ADDR_ACC0_BRAM_L && addr <= VP_ADDR_ACC0_BRAM_H)
	{
		acc0_bram_socket->b_transport(pl, offset);
		//cout << "poslata vrednost: " << (int) buf << ", u funkciju usla: " << val << endl; 
	}
	else if (addr >= VP_ADDR_ACC1_BRAM_L && addr <= VP_ADDR_ACC1_BRAM_H)
	{
		acc1_bram_socket->b_transport(pl, offset);
		//cout << "poslata vrednost: " << (int) buf << ", u funkciju usla: " << val << endl; 
	}
	else
	{
		SC_REPORT_ERROR("IP_HARD RAM", "Wrong write address.");
	}
}

unsigned char Ip_Core::read_bram(sc_dt::sc_uint<64> addr)
{
	pl_t pl;
	unsigned char buf[4];
	sc_dt::uint64 taddr = addr & 0x00FFFFFF;
	pl.set_address(taddr);
	pl.set_data_length(BUS_WIDTH); 
	pl.set_data_ptr(buf);
	pl.set_command(tlm::TLM_READ_COMMAND);
	pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );
	if (addr >= VP_ADDR_IMG_BRAM_L && addr <= VP_ADDR_IMG_BRAM_H)
	{
		img_bram_socket->b_transport(pl, offset);
	}
	else if (addr >= VP_ADDR_ACC0_BRAM_L && addr <= VP_ADDR_ACC0_BRAM_H)
	{
		acc0_bram_socket->b_transport(pl, offset);
	}
	else if (addr >= VP_ADDR_ACC1_BRAM_L && addr <= VP_ADDR_ACC1_BRAM_H)
	{
		acc1_bram_socket->b_transport(pl, offset);
	}
	else
	{
		SC_REPORT_ERROR("IP_HARD RAM", "Wrong read address.");
	}
	return buf[3];
}

trig_t Ip_Core::read_rom(sc_dt::sc_uint<64> addr)
{
	pl_t pl;
	sc_dt::sc_int <16> val;
	unsigned char buf[2];
	sc_dt::uint64 taddr = addr & 0x00FFFFFF; 
	pl.set_address(taddr);
	pl.set_data_length(2); 
	pl.set_data_ptr(buf);
	pl.set_command( tlm::TLM_READ_COMMAND );
	pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );
	if (addr >= VP_ADDR_SIN_ROM_L && addr <= VP_ADDR_SIN_ROM_H)
	{
		sin_rom_socket->b_transport(pl, offset);
	}
	else if (addr >= VP_ADDR_COS_ROM_L && addr <= VP_ADDR_COS_ROM_H)
	{
		cos_rom_socket->b_transport(pl, offset);
	}
	else
	{
		SC_REPORT_ERROR("IP_HARD ROM", "Wrong read address.");
	}
	pl.set_address(addr);
	
	return toTrig_t(buf);
}
