#include "rsa_control.h"

void rsa_control::thread0() {
	rsa_inp_rdy.write(0);
	rsa_outp_vld.write(0);
	en_in_vld.write(0);
	en_out_rdy.write(0);
	wait();

	while(true) {
		wait(1, SC_PS);
		
		rsa_inp_rdy.write(1);
		do {
			wait();
		} while(!rsa_inp_vld.read());
		N1 = N.read();
		M1 = M.read();
		d1 = d.read();
		e1 = e.read();
		crypto1 = crypto.read();
		rsa_inp_rdy.write(0);
		wait();

		if(crypto1 == NULL) {
			wait();
		}
		else if (crypto1) {
			en_in_vld.write(1);
			msg.write(M1);
			divisor.write(N1);
			key.write(e1);
			do {
				wait();
			} while (!en_in_rdy.read());
			en_in_vld.write(0);

			en_out_rdy.write(1);
			do {
				wait();
			} while (!en_out_vld.read());
			tmp = result.read();
			wait();
			en_out_rdy.write(0);
		}
		else if (!crypto) {
			en_in_vld.write(1);
			msg.write(M1);
			divisor.write(N1);
			key.write(d1);
			do {
				wait();
			} while (!en_in_rdy.read());
			en_in_vld.write(0);

			en_out_rdy.write(1);
			do {
				wait();
			} while (!en_out_vld.read());
			tmp = result.read();
			wait();
			en_out_rdy.write(0);
		}
		rsa_outp_vld.write(1);
		C.write(tmp);

		do {
			wait();
		} while (!rsa_outp_rdy.read());
		rsa_outp_vld.write(0);
	}
}