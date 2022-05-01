#include "systemc.h"
#include "mod_exp.h"

SC_MODULE(rsa_control) {
	sc_in_clk clk;
	sc_in<bigint> N;
	sc_in<bigint> d;
	sc_in<bigint> M;
	sc_in<bigint> e;
	sc_in<bigint> result;
	sc_in<bool> crypto;
	sc_in<bool> rsa_inp_vld;
	sc_in<bool> rsa_outp_rdy;
	sc_in<bool> en_in_rdy;
	sc_in<bool> en_out_vld;

	sc_out<bigint> divisor;
	sc_out<bigint> msg;
	sc_out<bigint> key;
	sc_out<bigint> C;
	sc_out<bool> rsa_inp_rdy;
	sc_out<bool> rsa_outp_vld;
	sc_out<bool> en_in_vld;
	sc_out<bool> en_out_rdy;

	sc_signal<bigint> tmp;
	sc_signal<bigint> N1;
	sc_signal<bigint> M1;
	sc_signal<bigint> d1;
	sc_signal<bigint> e1;
	sc_signal<bool> crypto1;

	void thread0();

	SC_CTOR(rsa_control) {
		SC_CTHREAD(thread0, clk.pos());
	}
};