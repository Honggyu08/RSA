#include "systemc.h"
#include "rsa_control.h"

extern sc_trace_file *fp;

SC_MODULE(rsa_top) {
	sc_in_clk clk;
	sc_in<bigint> N;
	sc_in<bigint> d;
	sc_in<bigint> e;
	sc_in<bigint> M;
	sc_in<bool> crypto;
	sc_in<bool> inp_vld;
	sc_in<bool> outp_rdy;

	sc_out<bigint> C;
	sc_out<bool> inp_rdy;
	sc_out<bool> outp_vld;

	sc_signal<bigint> tmp_N;
	sc_signal<bigint> tmp_d;
	sc_signal<bigint> tmp_e;
	sc_signal<bigint> tmp_M;
	sc_signal<bigint> tmp_C;

	sc_signal<bigint> result_sig;
	sc_signal<bigint> key_pri;
	sc_signal<bigint> key_pub;
	sc_signal<bigint> key_sig;
	sc_signal<bigint> msg_sig;
	sc_signal<bigint> msg_rsa_en;
	sc_signal<bigint> divisor_rsa_en;
	sc_signal<bigint> result_en_rsa;
	sc_signal<bigint> divisor_sig;
	sc_signal<bigint> C_sig;
	sc_signal<bool> rsa_inp_vld;
	sc_signal<bool> rsa_inp_rdy;
	sc_signal<bool> rsa_outp_vld;
	sc_signal<bool> rsa_outp_rdy;
	sc_signal<bool> en_in_rdy_sig;
	sc_signal<bool> en_in_vld_sig;
	sc_signal<bool> en_out_rdy_sig;
	sc_signal<bool> en_out_vld_sig;

	rsa_control *rsa;
	mod_exp *encrypt;

	void sc_top();
	
	SC_CTOR(rsa_top) {
		SC_CTHREAD(sc_top, clk.pos());

		rsa = new rsa_control("rsa");
		rsa->clk(clk);
		rsa->N(divisor_sig);
		rsa->d(key_pri);
		rsa->e(key_pub);
		rsa->M(msg_sig);
		rsa->result(result_en_rsa);
		rsa->crypto(crypto);
		rsa->rsa_inp_vld(rsa_inp_vld);
		rsa->rsa_outp_rdy(rsa_outp_rdy);
		rsa->en_in_rdy(en_in_rdy_sig);
		rsa->en_out_vld(en_out_vld_sig);
		
		rsa->divisor(divisor_rsa_en);
		rsa->msg(msg_rsa_en);
		rsa->key(key_sig);
		rsa->C(C_sig);
		rsa->rsa_inp_rdy(rsa_inp_rdy);
		rsa->rsa_outp_vld(rsa_outp_vld);
		rsa->en_in_vld(en_in_vld_sig);
		rsa->en_out_rdy(en_out_rdy_sig);

		encrypt = new mod_exp("encrypt");
		encrypt->clk(clk);
		encrypt->M(msg_rsa_en);
		encrypt->k(key_sig);
		encrypt->N(divisor_rsa_en);
		encrypt->exp_in_vld(en_in_vld_sig);
		encrypt->exp_out_rdy(en_out_rdy_sig);
		
		encrypt->C(result_en_rsa);
		encrypt->exp_in_rdy(en_in_rdy_sig);
		encrypt->exp_out_vld(en_out_vld_sig);

		sc_trace(fp, N, "top.N_top");
		sc_trace(fp, d, "top.d_top");
		sc_trace(fp, e, "top.e_top");
		sc_trace(fp, M, "top.M_top");
		sc_trace(fp, C, "top.C_top");
		sc_trace(fp, crypto, "top.crypto_top");
		sc_trace(fp, inp_vld, "top.inp_vld_top");
		sc_trace(fp, inp_rdy, "top.inp_rdy_top");
		sc_trace(fp, outp_vld, "top.outp_rdy_top");
		sc_trace(fp, outp_rdy, "top.outp_rdy_top");
		sc_trace(fp, rsa_inp_vld, "top.rsa_inp_vld_top");
		sc_trace(fp, rsa_inp_rdy, "top.rsa_inp_rdy_top");
		sc_trace(fp, rsa_outp_vld, "top.rsa_out_vld_top");
		sc_trace(fp, rsa_outp_rdy, "top.rsa_outp_rdy_top");

		sc_trace(fp, rsa->N, "top.rsa.N_rsa");
		sc_trace(fp, rsa->d, "top.rsa.d_rsa");
		sc_trace(fp, rsa->e, "top.rsa.e_rsa");
		sc_trace(fp, rsa->M, "top.rsa.M_rsa");
		sc_trace(fp, rsa->C, "top.rsa.C_rsa");
		sc_trace(fp, rsa->crypto, "top.rsa.crypto_rsa");
		sc_trace(fp, rsa->rsa_inp_vld, "top.rsa.rsa_inp_vld_rsa");
		sc_trace(fp, rsa->rsa_outp_rdy, "top.rsa.rsa_outp_rdy_rsa");
		sc_trace(fp, rsa->rsa_inp_rdy, "top.rsa.rsa_inp_rdy_rsa");
		sc_trace(fp, rsa->rsa_outp_vld, "top.rsa.rsa_outp_vld_rsa");
		sc_trace(fp, rsa->en_in_rdy, "top.rsa.en_in_rdy_rsa");
		sc_trace(fp, rsa->en_out_vld, "top.rsa.en_out_vld_rsa");
		sc_trace(fp, rsa->en_in_vld, "top.rsa.en_in_vld_rsa");
		sc_trace(fp, rsa->en_out_rdy, "top.rsa.en_out_rdy_rsa");

		sc_trace(fp, encrypt->M, "top.en.msg_en");
		sc_trace(fp, encrypt->k, "top.en.key_en");
		sc_trace(fp, encrypt->N, "top.en.N_en");
		sc_trace(fp, encrypt->C, "top.en.C_en");
		sc_trace(fp, encrypt->exp_in_vld, "top.en.en_in_vld_en");

		sc_trace(fp, encrypt->exp_out_rdy, "top.en.en_out_rdy_en");
		sc_trace(fp, encrypt->exp_in_rdy, "to.en.en_in_rdy_en");
		sc_trace(fp, encrypt->exp_out_vld, "top.en.en_out_vld_en");
	}
	~rsa_top(){
		delete rsa;
		delete encrypt;
	}
};