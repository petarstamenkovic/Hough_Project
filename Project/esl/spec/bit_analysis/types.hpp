#ifndef _FP_TYPES_HPP_
#define _FP_TYPES_HPP_

	#ifndef SC_INCLUDE_FX
	#define SC_INCLUDE_FX
	#endif

#include <systemc>

typedef sc_dt::sc_fixed_fast<16, 2> trig_t;
typedef sc_dt::sc_ufixed_fast<8, 1> dw_t;
typedef sc_dt::sc_fixed_fast<16, 12> point_t;
typedef sc_dt::sc_fixed_fast<24, 12> slope_t;
typedef sc_dt::sc_fixed_fast<28, 24> intercept_t;

#endif
