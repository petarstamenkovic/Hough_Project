#include "vp.hpp"

Vp::Vp (sc_core::sc_module_name name, char** strings, int argv): 
	sc_module (name),
	cpu("Cpu", strings, argv),
	interconnect("Interconnect"),
	ip_core("Ip_core"),
	sin_rom("Sin_rom"),
	cos_rom("Cos_rom"),
	img_bram("Img_bram"),
	acc0_bram("Acc0_bram"),
	acc1_bram("Acc1_bram")
{
	cpu.interconnect_socket.bind(interconnect.cpu_socket);
	
	interconnect.ip_core_socket.bind(ip_core.interconnect_socket);
	interconnect.img_bram_socket.bind(img_bram.interconnect_socket);
	interconnect.acc0_bram_socket.bind(acc0_bram.interconnect_socket);
	interconnect.acc1_bram_socket.bind(acc1_bram.interconnect_socket);
	
	ip_core.sin_rom_socket.bind(sin_rom.ip_core_socket);
	ip_core.cos_rom_socket.bind(cos_rom.ip_core_socket);
	ip_core.img_bram_socket.bind(img_bram.ip_core_socket);
	ip_core.acc0_bram_socket.bind(acc0_bram.ip_core_socket);
	ip_core.acc1_bram_socket.bind(acc1_bram.ip_core_socket);

	SC_REPORT_INFO("Virtual Platform", "Constructed.");
}

Vp::~Vp()
{
 	SC_REPORT_INFO("Virtual Platform", "Destroyed.");
}


