#include "functions.hpp"

#include <iostream>

void bufToBuf(unsigned char *val, unsigned char *buf)
{
    val[0] = buf[0];
    val[1] = buf[1];
    val[2] = buf[2];
    val[3] = buf[3];
}

int toInt(unsigned char *buf)
{
    int val = 0;
    val += ((int)buf[0]) << 24;
    val += ((int)buf[1]) << 16;
    val += ((int)buf[2]) << 8;
    val += ((int)buf[3]);
    return val;
}

int toInt2(unsigned char *buf)
{
    int val = 0;
    val += ((int)buf[0]) << 8;
    val += ((int)buf[1]);
    return val;
}

trig_t toTrig_t(unsigned char *buf)
{
    uint16_t val = 0;

    val += (buf[0]) << 8;
    val += buf[1];
    //std::cout << "Provera sta dolazi u funkciju " << val << " " << std::hex << (int)buf[0] << " " << std::hex << (int)buf[1] << " " <<  (trig_t) ((double) val / 16384) << std::endl ;
    return ((double) val / 16384);
}

void toUchar4(unsigned char *buf,int val)
{
    buf[0] = (char) (val >> 24);
    buf[1] = (char) (val >> 16);
    buf[2] = (char) (val >> 8);
    buf[3] = (char) (val);
}

void toUchar2(unsigned char *buf, int val)
{
    buf[0] = (unsigned char) (val >> 8);
    buf[1] = (unsigned char) (val);
}

void toUchar1(unsigned char *buf, int val)
{
    buf[0] = (unsigned char) (val);
}

void toUcharr(unsigned char *buf, uint8_t val)
{
	*buf = (unsigned char) (val);
}

unsigned char to_U(sc_dt::sc_uint<8>  val)
{
    unsigned char buf = (unsigned) val;
    return buf;
} 
