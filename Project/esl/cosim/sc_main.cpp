#define SC_MAIN

#include <systemc>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include "cpu.hpp"
#include "interconnect.hpp"
#include "ip_core.hpp"
#include "address.hpp"

using namespace sc_core;

int sc_main(int argc, char* argv[])
{
	Cpu cpu("Cpu", argv, argc);
	Interconnect interconnect("Interconnect");
	Ip_Core ip_core("Ip_core");

	cpu.interconnect_socket.bind(interconnect.cpu_socket);
	interconnect.ip_core_socket.bind(ip_core.interconnect_socket);

	sc_start();
   
	return 0;
}
