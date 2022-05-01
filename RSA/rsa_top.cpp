#include "rsa_top.h"

void rsa_top::sc_top() {
	inp_rdy.write(0);
	outp_vld.write(0);
	rsa_inp_vld.write(0);
	rsa_outp_rdy.write(0);
	wait();
	
	while(true) {
		wait(1, SC_PS);

		inp_rdy.write(1);
		do{
			wait();
		} while(!inp_vld.read());
		
		tmp_N = N.read();
		tmp_d = d.read();
		tmp_e = e.read();
		tmp_M = M.read();
		inp_rdy.write(0);
		wait();

		rsa_inp_vld.write(1);
		divisor_sig = tmp_N;
		key_pri = tmp_d;
		key_pub = tmp_e;
		msg_sig = tmp_M;
		do {
			wait();
		} while (!rsa_inp_rdy.read());
		rsa_inp_vld.write(0);

		rsa_outp_rdy.write(1);
		do {
			wait();
		} while (!rsa_outp_vld.read());
		tmp_C = C_sig;
		wait();
		rsa_outp_rdy.write(0);

		outp_vld.write(1);
		C.write(tmp_C);
		do {
			wait();
		} while (!outp_rdy.read());
		outp_vld.write(0);
	}
}