/*
 * @file tools.c
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
#include <stdio.h>
#include <gmp.h>
#include "tools.h"

/**
 * The function prints debug messages to stderr; it is compiled out if PAILLIER_DEBUG is not defined.
 */
inline int debug_msg(const char *str) {
#ifdef PAILLIER_DEBUG
	return fputs(str, stderr);
#endif
}

/**
 * Generate a random number using /dev/urandom
 */
int gen_random(mpz_t rnd, mp_bitcnt_t len) {
	FILE *dev_urandom;
	int byte_count, byte_read;
	char * seed;

	byte_count = BIT2BYTE(len);

	dev_urandom = fopen("/dev/urandom", "r");
	if(dev_urandom == NULL) {
		fprintf(stderr, "cannot open random number device!\n");
		exit(1);
	}

	seed = (char *)malloc(sizeof(char)*byte_count);

	byte_read = 0;
	//generate the bytes with /dev/urandom
	while(byte_read < byte_count) {
		byte_read += fread(seed, sizeof(char), byte_count, dev_urandom);
	}
	fclose(dev_urandom);

	mpz_import(rnd, byte_count, 1, sizeof(seed[0]), 0, 0, seed);
	free(seed);
	return 0;
}

/**
 * Generate a random prime number using /dev/random as a source of randomness.
 * Since /dev/random is blocking, you may need to move your mouse to feed randomness.
 */
int gen_prime(mpz_t prime, mp_bitcnt_t len) {
	FILE * dev_random, *dev_urandom;
	int byte_count, byte_read;
	char * seed;
	mpz_t rnd;

	mpz_init2(rnd, len);
	byte_count = BIT2BYTE(len);

	dev_random = fopen("/dev/random", "r");
	if(dev_random == NULL) {
		fprintf(stderr, "cannot open random number device!\n");
		exit(1);
	}

	seed = (char *)malloc(sizeof(char)*byte_count);

	byte_read = 0;
	//generate the bytes with dev/random
	while(byte_read < byte_count) {
		byte_read += fread(seed, sizeof(char), byte_count, dev_random);
	}
	fclose(dev_random);

	mpz_import(rnd, byte_count, 1, sizeof(seed[0]), 0, 0, seed);
	//set most significant bit to 1
	mpz_setbit(rnd, len-1);
	//look for next prime
	mpz_nextprime(prime, rnd);

	mpz_clear(rnd);
	free(seed);
	return 0;
}

/**
 * The exponentiation is computed using Garner's method for the CRT:
 * - Exponentiation mod p: y_p = (x mod p)^{exp_p} mod p
 * - Exponentiation mod q: y_q = (x mod q)^{exp_q} mod q
 * - Recombination: y = y_p + p*(p^{-1} mod q)*(y_q-y_p) mod n
 */
int crt_exponentiation(mpz_t result, mpz_t base, mpz_t exp_p, mpz_t exp_q, mpz_t pinvq, mpz_t p, mpz_t q) {
	mpz_t res_p, res_q, base_p, base_q, pq;
	mpz_init(res_p);
	mpz_init(res_q);
	mpz_init(base_p);
	mpz_init(base_q);
	mpz_init(pq);

	//compute exponentiation modulo p
	mpz_mod(base_p, base, p);
	mpz_powm(res_p, base_p, exp_p, p);

	//compute exponentiation modulo q
	mpz_mod(base_q, base, q);
	mpz_powm(res_q, base_q, exp_q, q);

	//recombination
	mpz_mul(pq, p, q);
	mpz_sub(result, res_q, res_p);
	mpz_mul(result, result, p);
	mpz_mul(result, result, pinvq);
	mpz_add(result, result, res_p);
	mpz_mod(result, result, pq);

	mpz_clear(res_p);
	mpz_clear(res_q);
	mpz_clear(base_p);
	mpz_clear(base_q);
	mpz_clear(pq);

	return 0;
}
