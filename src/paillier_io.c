/**
 * @file paillier_io.c
 *
 * @date Created on: Sep 06, 2012
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

#include "paillier.h"

/**
 * Wrapper to the key generation function using stdio streams as inputs and output.
 * @see paillier_keygen
 */
int paillier_keygen_str(FILE *public_key, FILE *private_key, int len) {
	int result;
	paillier_public_key pub;
	paillier_private_key priv;

	paillier_public_init(&pub);
	paillier_private_init(&priv);

	//generate keys
	result = paillier_keygen(&pub, &priv, len);

	//export public key
	debug_msg("export public key: \n");
	result |= paillier_public_out_str(public_key, &pub);

	//export private key
	debug_msg("export private key: \n");
	result |= paillier_private_out_str(private_key, &priv);

	debug_msg("freeing memory\n");
	paillier_public_clear(&pub);
	paillier_private_clear(&priv);

	debug_msg("exiting\n");
	return result;
}

/**
 * Wrapper to the encryption function using stdio streams as inputs and output.
 * @see paillier_encrypt
 */
int paillier_encrypt_str(FILE *ciphertext, FILE *plaintext, FILE *public_key) {
	mpz_t c, m;
	int result;
	paillier_public_key pub;

	mpz_init(c);
	mpz_init(m);
	paillier_public_init(&pub);

	//import public key
	debug_msg("importing public key: \n");
	paillier_public_in_str(&pub, public_key);

	//convert plaintext from stream
	debug_msg("importing plaintext: \n");
	gmp_fscanf(plaintext, "%Zx\n", m);
	if(mpz_cmp(m, pub.n) >= 0) {
		fputs("Warning, plaintext is larger than modulus n!\n", stderr);
	}

	//calculate encryption
	result = paillier_encrypt(c, m, &pub);

	//convert ciphertext to stream
	debug_msg("exporting ciphertext: \n");
	gmp_fprintf(ciphertext, "%Zx\n", c);

	debug_msg("freeing memory\n");
	mpz_clear(c);
	mpz_clear(m);
	paillier_public_clear(&pub);

	debug_msg("exiting\n");
	return result;
}

/**
 * Wrapper to the decryption function using stdio streams as inputs and output.
 * @see paillier_decrypt
 */
int paillier_decrypt_str(FILE *plaintext, FILE *ciphertext, FILE *private_key) {
	mpz_t c, m, n2;
	paillier_private_key priv;
	int result;

	mpz_init(c);
	mpz_init(m);
	mpz_init(n2);
	paillier_private_init(&priv);

	//import private key
	debug_msg("importing private key: \n");
	paillier_private_in_str(&priv, private_key);

	//compute n^2
	mpz_mul(n2, priv.n, priv.n);

	//convert ciphertext from stream
	debug_msg("importing ciphertext: \n");
	gmp_fscanf(ciphertext, "%Zx\n", c);
	if(mpz_cmp(c, n2) >= 0) {
		fputs("Warning, ciphertext is larger than modulus n^2!\n", stderr);
	}
	//calculate decryption
	result = paillier_decrypt(m, c, &priv);

	//convert plaintext to stream
	debug_msg("exporting plaintext: \n");
	gmp_fprintf(plaintext, "%Zx\n", m);

	debug_msg("freeing memory\n");
	mpz_clear(c);
	mpz_clear(m);
	mpz_clear(n2);
	paillier_private_clear(&priv);

	debug_msg("exiting\n");
	return result;
}


