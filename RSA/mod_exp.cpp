#include "mod_exp.h"

void mod_exp::sc_mod_exp() {
	result_reg = 1;
	state = true;
	base_next = 0;
	result_next = 0;
	exponent_next = 0;
	C.write(0);
	base_mod_in_vld.write(0);
	base_mod_out_rdy.write(0);
	result_mod_in_vld.write(0);
	result_mod_out_rdy.write(0);
	exp_in_rdy.write(0);
	exp_out_vld.write(0);
	wait();

	while(true) {
		wait(1, SC_PS);

		exp_in_rdy.write(1);
		do{
			wait();
		} while(!exp_in_vld.read());
		base_reg = M.read();
		exponent_reg = k.read();
		modulo_reg = N.read();
		wait();
		exp_in_rdy.write(0);

		for (int i = 0; i < NBITS; i++) {
			base_mod_in_vld.write(10);
			result_mod_in_vld.write(10);
			base_squared = base_reg * base_reg;
			result_base = result_reg * base_reg;
			do {
				wait();
			} while (!(base_mod_in_rdy.read() & result_mod_in_rdy.read()));
			base_mod_in_vld.write(0);
			result_mod_in_vld.write(0);
			exponent_next = exponent_reg >> 1;

			base_mod_out_rdy.write(1);
			result_mod_out_rdy.write(1);
			do{
				wait();
			} while(!(base_mod_out_vld.read() & result_mod_out_vld.read()));
			base_mod_out_rdy.write(0);
			result_mod_out_rdy.write(0);

			switch (state) {
				case true:
					if (exponent_reg != 0){
						if(exponent_reg[0] & 1) {
							result_reg = result_next;
						}
						base_reg = base_next;
						exponent_reg = exponent_next;
						state = true;
					}
					else {
						state = false;
					}
					break;
				case false:
					break;
			}
			if(!state) {
				break;
			}
		}
		if(result_reg < 0) {
			result_reg = result_reg + N;
		}
		exp_out_vld.write(1);
		C.write(result_reg);
		do {
			wait();
		} while (!exp_out_rdy.read());
		exp_out_vld.write(0);
	}
}