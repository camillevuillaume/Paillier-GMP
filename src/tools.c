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

int gen_random(mpz_t rnd, mp_bitcnt_t bits) {
	FILE *dev_urandom;
	int byte_count, byte_read;
	char * seed;

	byte_count = (bits+7) >> 3;

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

int gen_prime(mpz_t prime, mp_bitcnt_t bits) {
	FILE * dev_random, *dev_urandom;
	int byte_count, byte_read;
	char * seed;
	mpz_t rnd;

	mpz_init2(rnd, bits);
	byte_count = (bits+7) >> 3;

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
	mpz_setbit(rnd, bits-1);
	//look for next prime
	mpz_nextprime(prime, rnd);

	mpz_clear(rnd);
	free(seed);
	return 0;
}

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
