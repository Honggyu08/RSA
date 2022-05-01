#include "tb.h"
#include "key.h"

#define DEBUG_SYSTEMC

void tb::source() {
	bigint M1;
	bigint N1;
	bigint d1;
	bigint e1;

    //generate key
    // Generate all 1's in r.
    char r_str[ HALF_STR_SIZE + 1 ];

    max_bitstr( r_str, HALF_STR_SIZE );
    bigint r = r_str;

#ifdef DEBUG_SYSTEMC
    assert( r > 0 );
#endif

    // Find two large primes p and q.
    bigint p = find_prime( r );
    bigint q = find_prime( r );

#ifdef DEBUG_SYSTEMC
    assert( ( p > 0 ) && ( q > 0 ) );
#endif

    // Compute n and ( p - 1 ) * ( q - 1 ) = m.
    N1 = p * q;
    bigint m = ( p - 1 ) * ( q - 1 );

#ifdef DEBUG_SYSTEMC
    assert( ( N1 > 0 ) && ( m > 0 ) );
#endif
    // Find a small odd integer e that is relatively prime to m.
    e1 = find_rel_prime( m );

#ifdef DEBUG_SYSTEMC
    assert( e1 > 0 );
#endif

    // Find the multiplicative inverse d of e, modulo m.
    d1 = inverse( e1, m );

#ifdef DEBUG_SYSTEMC
    assert( d1 > 0 );
#endif

    // Output public and secret keys.
    cout << "RSA public key P: P=( e, n )" << endl;
    cout << "e = " << e1 << endl;
    cout << "n = " << N1 << endl;
    cout << endl;

    cout << "RSA secret key S: S=( d, n )" << endl;
    cout << "d = " << d1 << endl;
    cout << "n = " << N1 << endl;
    cout << endl;
    //end generation

	crypto.write(NULL);
	inp_vld.write(0);
	wait();
	
	while(true) {
		wait(1, SC_PS);

        // Cipher and decipher a randomly generated message msg.
        char msg_str[ STR_SIZE + 1 ];
        rand_bitstr( msg_str, STR_SIZE );
        M1 = msg_str;

        M1 %= N1; // Make sure msg is smaller than n. If larger, this part
              // will be a block of the input message.

#ifdef DEBUG_SYSTEMC
    assert( M1 > 0 );
#endif

		inp_vld.write(1);
		N.write(N1);
		d.write(d1);
		e.write(e1);
		M.write(M1);
		crypto.write(true);

		cout << sc_time_stamp() << " Message to be chiphered: " << M.read() << "\n" << flush;
		do {
			wait();
		}while (!inp_rdy.read());
		inp_vld.write(0);
	}
}

void tb::sink() {
	bigint M_c;
	outp_rdy.write(0);

	while(true) {
		wait(1, SC_PS);

		outp_rdy.write(1);
		do {
			wait();
		} while (!outp_vld.read());

		M_c = C.read();
		wait();

		if(crypto.read()) {
			cout << sc_time_stamp() << " Ciphered Message: " << M_c << "\n\n" << flush;
		}
		else {
			cout << sc_time_stamp() << " Decihphered Message: " << M_c << "\n\n" << flush;
		}

		outp_rdy.write(0);
	}
}