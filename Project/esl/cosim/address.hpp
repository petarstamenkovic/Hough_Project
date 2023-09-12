#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#ifndef SC_INCLUDE_FX
#define SC_INCLUDE_FX
#endif

#include <systemc>
#include <tlm>

typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
typedef tlm::tlm_base_protocol_types::tlm_phase_type ph_t;

//registri u IP_Core
#define ADDR_START 0x00
#define ADDR_RESET 0x04
#define ADDR_WIDTH 0x08
#define ADDR_HEIGHT 0x0c
#define ADDR_RHO 0x10
#define ADDR_THRESHOLD 0x14
#define ADDR_READY 0x18

//velicina acc_bram-a 
#define ACC_BRAM_SIZE 0x3FFFF

//velicina img_bram-a
#define IMG_BRAM_SIZE 0x7FFFF

//ofset
#define DELAY 8

//sirina magistrale, 4 bajta
#define BUS_WIDTH 4

//adrese 
#define VP_ADDR_ACC0_BRAM_L 0x40000000
#define VP_ADDR_ACC0_BRAM_H 0x40000000 + ACC_BRAM_SIZE
#define VP_ADDR_ACC1_BRAM_L 0x42000000
#define VP_ADDR_ACC1_BRAM_H 0x42000000 + ACC_BRAM_SIZE
#define VP_ADDR_IMG_BRAM_L 0x44000000
#define VP_ADDR_IMG_BRAM_H 0x44000000 + IMG_BRAM_SIZE
#define VP_ADDR_IP_HARD_L 0x43C00000
#define VP_ADDR_IP_HARD_H 0x43C0FFFF

#endif 
