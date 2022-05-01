#include "systemc.h"
#include <fstream>

#define NBITS 512
#define d_NBITS 2 * NBITS

typedef sc_bigint<NBITS> bigint;
typedef sc_bigint<d_NBITS> d_bigint;

SC_MODULE(mod) {
	sc_in_clk clk;
	sc_in<d_bigint> base;
	sc_in<bigint> N;
	sc_in<bool> mod_in_vld;
	sc_in<bool> mod_out_rdy;

	sc_out<bigint> R;
	sc_out<bool> mod_in_rdy;
	sc_out<bool> mod_out_vld;

	bigint r;
	d_bigint Q;

	void sc_mod();

	SC_CTOR(mod) {
		SC_CTHREAD(sc_mod, clk.pos());
	}
};