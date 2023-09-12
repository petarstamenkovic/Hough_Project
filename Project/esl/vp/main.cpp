#include "vp.hpp"
#include "address.hpp"

using namespace sc_core;


int sc_main(int argc, char* argv[])
{
    Vp vp("Virtual Platform", argv, argc);
    sc_start(1000, SC_NS);
   
    return 0;
}  
