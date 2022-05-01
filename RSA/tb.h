#include "systemc.h"
#include <fstream>

#define NBITS 512
#define d_NBITS 2 * NBITS

typedef sc_bigint<NBITS> bigint;
typedef sc_bigint<d_NBITS> d_bigint;

SC_MODULE(tb) {
	sc_in_clk clk;
	sc_in<bigint> C;
	sc_in<bool> inp_rdy;
	sc_in<bool> outp_vld;

	sc_out<bigint> N;
	sc_out<bigint> d;
	sc_out<bigint> e;
	sc_out<bigint> M;
	sc_out<bool> crypto;
	sc_out<bool> inp_vld;
	sc_out<bool> outp_rdy;

	void source();
	void sink();

	SC_CTOR(tb) {
		SC_CTHREAD(source, clk.pos());
		SC_CTHREAD(sink, clk.pos());
	}
};