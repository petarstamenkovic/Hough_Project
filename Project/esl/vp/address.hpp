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
#define ADDR_RESET 0x01
#define ADDR_IMG_WIDTH 0x02
#define ADDR_IMG_HEIGHT 0x03
#define ADDR_THRESHOLD 0x04
#define ADDR_RHO 0x05
#define ADDR_READY 0x06

//velicina acc_bram-a 
#define ACC_BRAM_SIZE 0xE916

//velicina img_bram-a
#define IMG_BRAM_SIZE 0x18000

//velicina jednog ROM-a
#define ROM_SIZE 0x168

//ofset
#define DELAY 8

//sirina magistrale, 4 bajta
#define BUS_WIDTH 4

//adrese 
#define VP_ADDR_SIN_ROM_L 0x00000000
#define VP_ADDR_SIN_ROM_H 0x00000000 + ROM_SIZE
#define VP_ADDR_COS_ROM_L 0x10000000
#define VP_ADDR_COS_ROM_H 0x10000000 + ROM_SIZE
#define VP_ADDR_ACC0_BRAM_L 0x20000000
#define VP_ADDR_ACC0_BRAM_H 0x20000000 + ACC_BRAM_SIZE
#define VP_ADDR_ACC1_BRAM_L 0x25000000
#define VP_ADDR_ACC1_BRAM_H 0x25000000 + ACC_BRAM_SIZE
#define VP_ADDR_IMG_BRAM_L 0x30000000
#define VP_ADDR_IMG_BRAM_H 0x30000000 + IMG_BRAM_SIZE
#define VP_ADDR_IP_HARD_L 0x40000000
#define VP_ADDR_IP_HARD_H 0x4000000F

#endif 
