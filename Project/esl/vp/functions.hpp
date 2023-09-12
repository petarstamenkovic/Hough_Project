#ifndef _FUNCTIONS_HPP
#define _FUNCTIONS_HPP

#ifndef SC_INCLUDE_FX
#define SC_INCLUDE_FX
#endif

#include <systemc>
#include <sysc/datatypes/fx/sc_fixed.h>
#include <iostream>
#include <string.h>
#include <bitset>
#include <math.h>
#include <string>
#include <systemc>
#include <tlm>
#include "types.hpp"

using namespace std;

void bufToBuf(unsigned char *val, unsigned char *buf);

int toInt(unsigned char *buf);

int toInt2(unsigned char *buf);

trig_t toTrig_t(unsigned char *buf);


void toUchar1(unsigned char *buf,int val);
void toUchar2(unsigned char *buf,int val);
void toUchar4(unsigned char *buf,int val);

void toUcharr(unsigned char *buf, uint8_t val);

unsigned char to_U(sc_dt::sc_uint<8>  val);

#endif
