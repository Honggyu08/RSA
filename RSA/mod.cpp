#include "mod.h"

void shift(bigint& r, d_bigint& Q){
	int len = Q.length();

	r = r << 1;
	r[0] = Q[len - 1];
	Q = Q << 1;
}

void mod::sc_mod(){
	mod_in_rdy.write(0);
	mod_out_vld.write(0);
	R.write(0);
	wait();

	while(true) {
		wait(1, SC_PS);
		mod_in_rdy.write(1);
		do {
			wait();
		} while (!mod_in_vld.read());
		Q = base.read();
		r = 0;
		wait();
		int len = Q.length();
		mod_in_rdy.write(0);
		for (int i = 0; i < len; i++) {
			shift(r, Q);
			if(r < N) {
			}
			else {
				r = r - N;
				Q[0] = 1;
			}
		}
		mod_out_vld.write(1);
		R.write(r);
		do { 
			wait();
		} while (!mod_out_rdy.read());
		mod_out_vld.write(0);
	}
}