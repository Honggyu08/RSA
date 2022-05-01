#include "systemc.h"
#include "mod.h"

extern sc_trace_file *fp;

SC_MODULE(mod_exp){
	sc_in_clk clk;
	sc_in<bigint> M;
	sc_in<bigint> k;
	sc_in<bigint> N;
	sc_in<bool> exp_in_vld;
	sc_in<bool> exp_out_rdy;
	
	sc_out<bigint> C;
	sc_out<bool> exp_in_rdy;
	sc_out<bool> exp_out_vld;

	bigint base_reg;
	bigint result_reg;
	bigint exponent_reg;
	sc_signal<bigint> base_next;
	sc_signal<bigint> result_next;
	sc_signal<bigint> modulo_reg;
	sc_signal<d_bigint> result_base;
	sc_signal<d_bigint> base_squared;
	bigint exponent_next;
	sc_signal<bool> base_mod_in_vld;
	sc_signal<bool> base_mod_in_rdy;
	sc_signal<bool> base_mod_out_vld;
	sc_signal<bool> base_mod_out_rdy;
	sc_signal<bool> result_mod_in_vld;
	sc_signal<bool> result_mod_in_rdy;
	sc_signal<bool> result_mod_out_vld;
	sc_signal<bool> result_mod_out_rdy;
	bool state;

	mod *base_squared_mod;
	mod *result_base_mod;

	void sc_mod_exp();

	SC_CTOR(mod_exp){
		SC_CTHREAD(sc_mod_exp, clk.pos());

		base_squared_mod = new mod("base_squared_mod");
		base_squared_mod->clk(clk);
		base_squared_mod->mod_in_vld(base_mod_in_vld);
		base_squared_mod->mod_in_rdy(base_mod_in_rdy);
		base_squared_mod->mod_out_vld(base_mod_out_vld);
		base_squared_mod->mod_out_rdy(base_mod_out_rdy);
		base_squared_mod->base(base_squared);
		base_squared_mod->N(modulo_reg);
		base_squared_mod->R(base_next);

		result_base_mod = new mod("result_base_mod");
		result_base_mod->clk(clk);
		result_base_mod->mod_in_vld(base_mod_in_vld);
		result_base_mod->mod_in_rdy(base_mod_in_rdy);
		result_base_mod->mod_out_vld(base_mod_out_vld);
		result_base_mod->mod_out_rdy(base_mod_out_rdy);
		result_base_mod->base(base_squared);
		result_base_mod->N(modulo_reg);
		result_base_mod->R(base_next);

		sc_trace(fp, base_mod_in_vld, "top.en.base_mod_in_vld_en");
		sc_trace(fp, base_mod_in_rdy, "top.en.base_mod_in_rdy_en");
		sc_trace(fp, base_mod_out_vld, "top.en.base_mod_out_vld_en");
		sc_trace(fp, base_mod_out_rdy, "top.en.out_rdy_en");
		sc_trace(fp, result_mod_in_vld, "top.en.result_mod_in_vld_en");
		sc_trace(fp, result_mod_in_rdy, "top.en.result_mod_in_rdy_en");
		sc_trace(fp, result_mod_out_vld, "top.en.result_mod_out_vld");
		sc_trace(fp, result_mod_out_rdy, "top.en.reuslt_mod_out_rdy");

		sc_trace(fp, base_squared_mod->base, "top.en.base.base_base");
		sc_trace(fp, base_squared_mod->N, "top.en.base.base_base");
		sc_trace(fp, base_squared_mod->R, "top.en.base.result_base");
		sc_trace(fp, base_squared_mod->mod_in_vld, "top.en.base.base_mod_in_vld");
		sc_trace(fp, base_squared_mod->mod_in_rdy, "top.en.base.base_mod_in_rdy");
		sc_trace(fp, base_squared_mod->mod_out_vld, "top.en.base.base_mod_out_vld");
		sc_trace(fp, base_squared_mod->mod_out_rdy, "top.en.base.base_mod_out_rdy");

		sc_trace(fp, result_base_mod->base, "top.en.result.base_result");
		sc_trace(fp, result_base_mod->N, "top.en.result.result_base");
		sc_trace(fp, result_base_mod->R, "top.en.result.result_result");
		sc_trace(fp, result_base_mod->mod_in_vld, "top.en.result.result_mod_in_vld");
		sc_trace(fp, result_base_mod->mod_in_rdy, "top.en.result.result_mod_in_rdy");
		sc_trace(fp, result_base_mod->mod_out_vld, "top.en.result.result_mod_out_vld");
		sc_trace(fp, result_base_mod->mod_out_rdy, "top.en.result.result_mod_out_rdy");
	}
};