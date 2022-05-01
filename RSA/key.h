#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>    // drand48, srand48
#include "systemc.h"

#define DEBUG_SYSTEMC // #undef this to disable assertions.

// NBITS is the number of bits in n of public and secret keys P and
// S. HALF_NBITS is the number of bits in p and q, which are the prime
// factors of n.
#define NBITS         512
#define HALF_NBITS    ( NBITS / 2 )    

// +2 is for the format specifier '0b' to make the string binary.
#define STR_SIZE      ( NBITS + 2 ) 
#define HALF_STR_SIZE ( HALF_NBITS + 2 )

typedef sc_bigint<NBITS>  bigint;

// Return the absolute value of x.
inline 
bigint
abs_val( const sc_signed& x ) 
{
  return ( x < 0 ? -x : x );
}

// Flip a coin with probability p.

#ifndef WIN32

inline
bool
flip( double p )
{
  return ( drand48() < p );
}

#else

inline
bool
flip( double p )
{
  const int MAX_VAL = ( 1 << 15 );

  // rand() produces an integer between 0 and 2^15-1, so rand() /
  // MAX_VAL is a number between 0 and 1, which is required to compare
  // with p.
  return ( rand() < ( int ) ( p * MAX_VAL ) );
}

#endif

// Randomly generate a bit string with nbits bits.  str has a length
// of nbits + 1. This function is used to generate random messages to
// process.
inline
void
rand_bitstr( char *str, int nbits )
{
  assert( nbits >= 4 );

  str[ 0 ] = '0';
  str[ 1 ] = 'b';
  str[ 2 ] = '0';  // Sign for positive numbers.

  for ( int i = 3; i < nbits; ++i )
    str[ i ] = ( flip( 0.5 ) == true ? '1' : '0' );

  str[ nbits ] = '\0';
}

// Generate "111..111" with nbits bits for masking.
// str has a length of nbits + 1.
inline
void
max_bitstr( char *str, int nbits )
{
  assert( nbits >= 4 );

  str[ 0 ] = '0';
  str[ 1 ] = 'b';  
  str[ 2 ] = '0';  // Sign for positive numbers.

  for ( int i = 3; i < nbits; ++i )
    str[ i ] = '1';

  str[ nbits ] = '\0';
}

// Return a positive remainder.
inline
bigint
ret_pos( const bigint& x, const bigint& n )
{
  if ( x < 0 )
    return x + n;
  return x;
}

// Compute the greatest common divisor ( gcd ) of a and b. This is
// Euclid's algorithm. This algorithm is at least 2,300 years old! The
// non-recursive version of this algorithm is not as elegant.
bigint
gcd( const bigint& a, const bigint& b )
{
  if ( b == 0 )
    return a;
  return gcd( b, a % b );
}

// Compute d, x, and y such that d = gcd( a, b ) = ax + by. x and y can
// be zero or negative. This algorithm is also Euclid's algorithm but
// it is extended to also find x and y. Recall that the existence of x
// and y is guaranteed by Euclid's algorithm.
void
euclid( const bigint& a, const bigint& b, bigint& d, bigint& x, bigint& y )
{
  if ( b != 0 ) {
    euclid( b, a % b, d, x, y );

    bigint tmp = x;
    x = y;
    y = tmp - ( a / b ) * y;
  }
  else {
    d = a;
    x = 1;
    y = 0;
  }
}

// Return the multiplicative inverse of a, modulo n, when a and n are
// relatively prime. Recall that x is a multiplicative inverse of a,
// modulo n, if a * x = 1 ( mod n ). 
inline
bigint
inverse( const bigint& a, const bigint& n )
{
  bigint d, x, y;

  euclid( a, n, d, x, y );
  assert( d == 1 );
  x %= n;

  return ret_pos( x, n );
}

// Find a small odd integer a that is relatively prime to n. I do not
// know an efficient algorithm to do that but the loop below seems to
// work; it usually iterates a few times. Recall that a is relatively
// prime to n if their only common divisor is 1, i.e., gcd( a, n ) ==
// 1.
inline
bigint
find_rel_prime( const bigint& n )
{
  bigint a = 3;
  while ( true ) {
    if ( gcd( a, n ) == 1 )
      break;
    a += 2;
#ifdef DEBUG_SYSTEMC
    assert( a < n );
#endif
  }

  return a;
}

// Return true if and only if a is a witness to the compositeness of
// n, i.e., a can be used to prove that n is composite.
inline
bool
witness( const bigint& a, const bigint& n )
{
  bigint n_minus1 = n - 1;
  bigint x;
  bigint d = 1;

  // Compute d = a^( n-1 ) % n.
  for ( int i = n.length() - 1; i >= 0; --i )
  {
    // Sun's SC5 bug when compiling optimized version
    // makes the wrong assignment if abs_val() is inlined
    //x = (sc_signed)d<0?-(sc_signed)d:(sc_signed)d;//abs_val( d );
    if(d<0)
      {
      x = -d;
      assert(x==-d);
      }
    else
      {
      x = d;
      assert(x==d);
      }
    d = ( d * d ) % n;

    // x is a nontrivial square root of 1 modulo n ==> n is composite.
    if ( ( abs_val( d ) == 1 ) && ( x != 1 ) && ( x != n_minus1 ) )
      return true;  

    if ( n_minus1[ i ] )
      d = ( d * a ) % n;
  }

  // d = a^( n-1 ) % n != 1 ==> n is composite.
  if ( abs_val( d ) != 1 )
    return true;  

  return false;
}

// Check to see if n has any small divisors. For small numbers, we do
// not have to run the Miller-Rabin primality test. We define "small"
// to be less than 1023. You can change it if necessary.
inline
bool
div_test( const bigint& n )
{
  int limit;

  if ( n < 1023 )
    limit = n.to_int() - 2;
  else
    limit = 1023;

  for ( int i = 3; i <= limit; i += 2 ) {
    if ( n % i == 0 )
      return false;   // n is composite.
  }

  return true;  // n may be prime.
}

// Return true if n is almost surely prime, return false if n is
// definitely composite.  This test, called the Miller-Rabin primality
// test, errs with probaility at most 2^(-s). CLR suggests s = 50 for
// any imaginable application, and s = 3 if we are trying to find
// large primes by applying miller_rabin to randomly chosen large
// integers. Even though we are doing the latter here, we will still
// choose s = 50. The probability of failure is at most
// 0.00000000000000088817, a pretty small number.
inline
bool
miller_rabin( const bigint& n )
{
  if ( n <= 2 )
    return false;

  if ( ! div_test( n ) )
    return false;
              
  char str[ STR_SIZE + 1 ];

  int s = 50;
  for ( int j = 1; j <= s; ++j ) {

    // Choose a random number.
    rand_bitstr( str, STR_SIZE );

    // Set a to the chosen number.
    bigint a = str;

    // Make sure that a is in [ 1, n - 1 ].
    a = ( a % ( n - 1 ) ) + 1;

    // Check to see if a is a witness.
    if ( witness( a, n ) )
      return false;  // n is definitely composite. 
  }

  return true;   // n is almost surely prime. 
}

// Return a randomly generated, large prime number using the
// Miller-Rabin primality test.
inline
bigint
find_prime( const bigint& r )
{
  char p_str[ HALF_STR_SIZE + 1 ];

  rand_bitstr( p_str, HALF_STR_SIZE );
  p_str[ HALF_STR_SIZE - 1 ] = '1';  // Force p to be an odd number.

  bigint p = p_str;

#ifdef DEBUG_SYSTEMC
  assert( ( p > 0 ) && ( p % 2 == 1 ) );
#endif

  // p is randomly determined. Now, we'll look for a prime in the
  // vicinity of p. By the prime number theorem, executing the
  // following loop approximately ln ( 2^NBITS ) iterations should
  // find a prime.

#ifdef DEBUG_SYSTEMC
  // A very large counter to check against infinite loops.
  sc_bigint<NBITS> niter = 0;
#endif

  while ( ! miller_rabin( p ) ) {
    p = ( p + 2 ) % r;

#ifdef DEBUG_SYSTEMC
    assert( ++niter > 0 );
#endif
  }

  return p;
}