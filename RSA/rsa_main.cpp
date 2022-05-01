#include <stdlib.h>
#include "systemc.h"
#include "rsa_top.h"
#include "tb.h"
#include <fstream>

sc_trace_file *fp = sc_create_vcd_trace_file("rsa_clock_no_log");

int sc_main(int argc, char *argv[]) {
	rsa_top *top;
	tb *tb0;

	sc_clock clock("clock", 10, SC_SEC, 0.5, 10, SC_SEC, true);
	sc_signal<bigint> N_sig;
	sc_signal<bigint> d_sig;
	sc_signal<bigint> e_sig;
	sc_signal<bigint> M_sig;
	sc_signal<bigint> C_sig;
	sc_signal<bool> crypto_sig;
	sc_signal<bool> inp_vld_sig;
	sc_signal<bool> inp_rdy_sig;
	sc_signal<bool> outp_vld_sig;
	sc_signal<bool> outp_rdy_sig;

	top = new rsa_top("top");
	top->clk(clock);
	top->N(N_sig);
	top->d(d_sig);
	top->e(e_sig);
	top->M(M_sig);
	top->crypto(crypto_sig);
	top->inp_vld(inp_vld_sig);
	top->outp_rdy(outp_rdy_sig);
	
	top->C(C_sig);
	top->inp_rdy(inp_rdy_sig);
	top->outp_vld(outp_vld_sig);

	tb0 = new tb("tb0");
	tb0->clk(clock);
	tb0->C(C_sig);
	tb0->inp_rdy(inp_rdy_sig);
	tb0->outp_vld(outp_vld_sig);
	
	tb0->N(N_sig);
	tb0->d(d_sig);
	tb0->e(e_sig);
	tb0->M(M_sig);
	tb0->crypto(crypto_sig);
	tb0->inp_vld(inp_vld_sig);
	tb0->outp_rdy(outp_rdy_sig);

	sc_trace(fp, clock, "clk");
	sc_trace(fp, N_sig, "N");
	sc_trace(fp, d_sig, "d");
	sc_trace(fp, e_sig, "e");
	sc_trace(fp, M_sig, "M");
	sc_trace(fp, crypto_sig, "crypto");
	sc_trace(fp, C_sig, "C");
	sc_trace(fp, inp_vld_sig, "inp_vld");
	sc_trace(fp, inp_rdy_sig, "inp_rdy");
	sc_trace(fp, outp_vld_sig, "outp_vld");
	sc_trace(fp, outp_rdy_sig, "outp_rdy");
	
	sc_start(650, SC_SEC);

	sc_close_vcd_trace_file(fp);

	delete top;
	delete tb0;

	return 0;
}