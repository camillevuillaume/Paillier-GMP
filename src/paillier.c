/**
 * @file paillier.c
 *
 * @date Created on: Aug 25, 2012
 * @author Camille Vuillaume
 * @copyright Camille Vuillaume, 2012
 *
 * This file is part of Paillier-GMP.
 *
 * Paillier-GMP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 *
 * Paillier-GMP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Paillier-GMP.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdlib.h>
#include "paillier.h"
#include "tools.h"

/** Function L(u)=(u-1)/n
 *
 * @ingroup Paillier
 * @param[out] result output result (u-1)/n
 * @param[in] input u
 * @param[in] ninv input n^{-1} mod 2^len
 * @param[in] len input bit length
 * @return 0 if no error
 *
 * The function L is evaluated using the pre-computed value n^{-1} mod 2^len.
 * The calculation a/n is computed as a*n^{-1} mod 2^len
 * - First a non-modular multiplication with n^{-1} mod 2^len is calculated.
 * - Then the result is reduced by masking higher bits.
 */
int paillier_ell(mpz_t result, mpz_t input, mpz_t ninv, mp_bitcnt_t len) {
	mpz_t mask;

	mpz_init(mask);

	mpz_sub_ui(result, input, 1);
	mpz_mul(result, result, ninv);
	mpz_setbit(mask, len);
	mpz_sub_ui(mask, mask, 1);
	mpz_and(result, result, mask);

	mpz_clear(mask);
	return 0;
}

/**
 * The function does the following.
 * - It generates two (probable) primes p and q having bits/2 bits.
 * - It computes the modulus n=p*q and sets the basis g to 1+n.
 * - It pre-computes n^{-1} mod 2^len.
 * - It pre-computes the CRT paramter p^{-2} mod q^2.
 * - It calculates lambda = lcm((p-1)*(q-1))
 * - It calculates mu = L(g^lambda mod n^2)^{-1} mod n using the CRT.
 * .
 * Since /dev/random is one of the sources of randomness in prime generation, the program may block.
 * In that case, you have to wait or move your mouse to feed /dev/random with fresh randomness.
 */
int paillier_keygen(paillier_public_key *pub, paillier_private_key *priv, mp_bitcnt_t len) {
	mpz_t p, q, n2, temp, mask, g;

	mpz_init(p);
	mpz_init(q);
	mpz_init(n2);
	mpz_init(temp);
	mpz_init(mask);
	mpz_init(g);

	//write bit lengths
	priv->len = len;
	pub->len = len;

	//generate p and q
	debug_msg("generating prime p\n");
	gen_prime(p, len/2);
	debug_msg("generating prime q\n");
	gen_prime(q, len/2);

	//calculate modulus n=p*q
	debug_msg("calculating modulus n=p*q\n");
	mpz_mul(pub->n, p, q);
	mpz_mul(priv->n, p, q);

	//set g = 1+n
	debug_msg("calculating basis g=1+n\n");
	mpz_add_ui(g, pub->n, 1);

	//compute n^{-1} mod 2^{len}
	debug_msg("computing modular inverse n^{-1} mod 2^{len}\n");
	mpz_setbit(temp, len);
	if(!mpz_invert(priv->ninv, pub->n, temp)) {
		fputs("Inverse does not exist!\n", stderr);
		mpz_clear(p);
		mpz_clear(q);
		mpz_clear(n2);
		mpz_clear(temp);
		mpz_clear(mask);
		mpz_clear(g);
		exit(1);
	}

	//compute p^2 and q^2
	mpz_mul(priv->p2, p, p);
	mpz_mul(priv->q2, q, q);

	//generate CRT parameter
	debug_msg("calculating CRT parameter p^{-2} mod q^2\n");
	mpz_invert(priv->p2invq2, priv->p2, priv->q2);

	//calculate lambda = lcm(p-1,q-1)
	debug_msg("calculating lambda=lcm((p-1)*(q-1))\n");
	mpz_clrbit(p, 0);
	mpz_clrbit(q, 0);
	mpz_lcm(priv->lambda, p, q);

	//calculate n^2
	mpz_mul(n2, pub->n, pub->n);

	//calculate mu
	debug_msg("calculating mu\n");
	crt_exponentiation(temp, g, priv->lambda, priv->lambda, priv->p2invq2, priv->p2, priv->q2);

	paillier_ell(temp, temp, priv->ninv, len);

	if(!mpz_invert(priv->mu, temp, pub->n)) {
		fputs("Inverse does not exist!\n", stderr);
		mpz_clear(p);
		mpz_clear(q);
		mpz_clear(n2);
		mpz_clear(temp);
		mpz_clear(mask);
		mpz_clear(g);
		exit(1);
	}

	//free memory and exit
	debug_msg("freeing memory\n");
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(n2);
	mpz_clear(temp);
	mpz_clear(mask);
	mpz_clear(g);
	debug_msg("exiting\n");
	return 0;
}

/**
 * The function calculates c=g^m*r^n mod n^2 with r random number.
 * Encryption benefits from the fact that g=1+n, because (1+n)^m = 1+n*m mod n^2.
 */
int paillier_encrypt(mpz_t ciphertext, mpz_t plaintext, paillier_public_key *pub) {
	mpz_t n2, r;

	if(mpz_cmp(pub->n, plaintext)) {
		mpz_init(n2);
		mpz_init(r);

		//re-compute n^2
		mpz_mul(n2, pub->n, pub->n);

		debug_msg("generating random number\n");
		//generate random r and reduce modulo n
		gen_pseudorandom(r, pub->len);
		mpz_mod(r, r, pub->n);
		if(mpz_cmp_ui(r, 0) == 0) {
			fputs("random number is zero!\n", stderr);
			mpz_clear(n2);
			mpz_clear(r);
			exit(1);
		}

		debug_msg("computing ciphertext\n");
		//compute r^n mod n2
		mpz_powm(ciphertext, r, pub->n, n2);

		//compute (1+m*n)
		mpz_mul(r, plaintext, pub->n);
		mpz_add_ui(r, r, 1);

		//multiply with (1+m*n)
		mpz_mul(ciphertext, ciphertext, r);
		mpz_mod(ciphertext, ciphertext, n2);

		debug_msg("freeing memory\n");
		mpz_clear(n2);
		mpz_clear(r);
	}
	debug_msg("exiting\n");
	return 0;
}

/**
 * The decryption function computes m = L(c^lambda mod n^2)*mu mod n.
 * The exponentiation is calculated using the CRT, and exponentiations mod p^2 and q^2 run in their own thread.
 *
 */
int paillier_decrypt(mpz_t plaintext, mpz_t ciphertext, paillier_private_key *priv) {
	debug_msg("computing plaintext\n");
	//compute exponentiation c^lambda mod n^2
	crt_exponentiation(plaintext, ciphertext, priv->lambda, priv->lambda, priv->p2invq2, priv->p2, priv->q2);

	//compute L(c^lambda mod n^2)
	paillier_ell(plaintext, plaintext, priv->ninv, priv->len);

	//compute L(c^lambda mod n^2)*mu mod n
	mpz_mul(plaintext, plaintext, priv->mu);
	mpz_mod(plaintext, plaintext, priv->n);

	debug_msg("exiting\n");
	return 0;
}

/**
 * "Add" two plaintexts homomorphically by multiplying ciphertexts modulo n^2.
 * For example, given the ciphertexts c1 and c2, encryptions of plaintexts m1 and m2,
 * the value c3=c1*c2 mod n^2 is a ciphertext that decrypts to m1+m2 mod n.
 */
int paillier_homomorphic_add(mpz_t ciphertext3, mpz_t ciphertext1, mpz_t ciphertext2, paillier_public_key *pub) {
	mpz_t n2;

	mpz_init(n2);
	debug_msg("compute n^2");
	mpz_mul(n2, pub->n, pub->n);

	debug_msg("homomorphic add plaintexts");
	mpz_mul(ciphertext3, ciphertext1, ciphertext2);
	mpz_mod(ciphertext3, ciphertext3, n2);

	debug_msg("freeing memory\n");
	mpz_clear(n2);
	debug_msg("exiting\n");
	return 0;
}

/**
 * "Multiplies" a plaintext with a constant homomorphically by exponentiating the ciphertext modulo n^2 with the constant as exponent.
 * For example, given the ciphertext c, encryptions of plaintext m, and the constant 5,
 * the value c3=c^5 n^2 is a ciphertext that decrypts to 5*m mod n.
 */
int paillier_homomorphic_multc(mpz_t ciphertext2, mpz_t ciphertext1, mpz_t constant, paillier_public_key *pub) {
	mpz_t n2;

	mpz_init(n2);
	debug_msg("compute n^2");
	mpz_mul(n2, pub->n, pub->n);

	debug_msg("homomorphic multiplies plaintext with constant");
	mpz_powm(ciphertext2, ciphertext1, constant, n2);

	debug_msg("freeing memory\n");
	mpz_clear(n2);
	debug_msg("exiting\n");
	return 0;
}
