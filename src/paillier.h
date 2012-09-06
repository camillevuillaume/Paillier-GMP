/**
 * @file paillier.h
 *
 * @date 		Created on: Aug 25, 2012
 * @author 		Camille Vuillaume
 * @copyright 	Camille Vuillaume, 2012
 * @defgroup 	Paillier Paillier
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

#ifndef PAILLIER_H_
#define PAILLIER_H_

#include <stdio.h>
#include <gmp.h>

/** Private key
 *
 * @ingroup Paillier
 */
typedef struct {
	mp_bitcnt_t bitlen; /**< bit length of n */
	mpz_t lambda;		/**< least common multiple of p and q */
	mpz_t mu;			/**< Modular inverse */
	mpz_t p2;			/**< square of prime number p */
	mpz_t q2;			/**< square of prime number q */
	mpz_t p2invq2;		/**< p^{-2} mod q^2 */
	mpz_t ninv;			/**< n^{-1} mod 2^l */
	mpz_t n;			/**< n=p*q */
} paillier_private_key;

/** Public key
 *
 * @ingroup Paillier
 *
 * The generator is 1+n.
 */
typedef struct {
	mp_bitcnt_t bitlen; /**< bit length of n */
	mpz_t n; 			/**< modulus */
} paillier_public_key;

/** Memory allocation for public key
 *
 * @ingroup Paillier
 * @param[in] pub input public key
 */
void paillier_public_init(paillier_public_key *pub);

/** Memory allocation for private key
 *
 * @ingroup Paillier
 * @param[in] priv input private key
 */
void paillier_private_init(paillier_private_key *priv);

/** Free memory for public key
 *
 * @ingroup Paillier
 * @param[in] pub input public key
 */
void paillier_public_clear(paillier_public_key *pub);

/** Free memory for private key
 *
 * @ingroup Paillier
 * @param[in] priv input private key
 */
void paillier_private_clear(paillier_private_key *priv);


/** Output public key
 *
 * @ingroup Paillier
 * @param[out] fp output stream
 * @param[in] pub input public key
 */
int paillier_public_out_str(FILE *fp, paillier_public_key *pub);

/** Output private key
 *
 * @ingroup Paillier
 * @param[out] fp output stream
 * @param[in] priv input private key
 */
int paillier_private_out_str(FILE *fp, paillier_private_key *priv);

/** Input public key
 *
 * @ingroup Paillier
 * @param[out] pub output public key
 * @param[in] fp input stream
 */
int paillier_public_in_str(paillier_public_key *pub, FILE *fp);

/** Input private key
 *
 * @ingroup Paillier
 * @param[out] priv output private key
 * @param[in] fp input stream
 */
int paillier_private_in_str(paillier_private_key *priv, FILE *fp);

/** Key generation
 *
 * @ingroup Paillier
 * @param[out] pub output public key
 * @param[out] priv output private key
 * @param[in] bits input bit length of public modulus
 */
int paillier_keygen(
		paillier_public_key *pub,
		paillier_private_key *priv,
		mp_bitcnt_t bits);


/** Key generation to stdio stream
 *
 * @ingroup Paillier
 * @param[out] public_key output stream for public key
 * @param[out] private_key output stream for private key
 * @param[in] bits input bit length of public modulus
 */
int paillier_keygen_str(
		FILE *public_key,
		FILE *private_key,
		int bits);

/** Encrypt
 *
 * @ingroup Paillier
 * @param[out] ciphertext output ciphertext c=g^m*r^n mod n^2
 * @param[in] plaintext input plaintext m
 * @param[in] pub input public key
 */
int paillier_encrypt(
		mpz_t ciphertext,
		mpz_t plaintext,
		paillier_public_key *pub);

/** Encrypt from stdio stream
 *
 * @ingroup Paillier
 * @param[out] ciphertext output stream for ciphertext c=g^m*r^n mod n^2
 * @param[in] plaintext input stream for plaintext m
 * @param[in] public_key input stream for public key
 */
int paillier_encrypt_str(
		FILE *ciphertext,
		FILE *plaintext,
		FILE *public_key);


/** Decrypt
 *
 * @ingroup Paillier
 * @param[out] plaintext output plaintext m
 * @param[in] ciphertext input ciphertext
 * @param[in] priv input private key
 */
int paillier_decrypt(
		mpz_t plaintext,
		mpz_t ciphertext,
		paillier_private_key *priv);

/** Decrypt from stdio stream
 *
 * @ingroup Paillier
 * @param[out] plaintext output stream for plaintext m
 * @param[in] ciphertext input stream for ciphertext c
 * @param[in] private_key input stream for private key
 */
int paillier_decrypt_str(
		FILE *ciphertext,
		FILE *plaintext,
		FILE *private_key);


#endif /* PAILLIER_H_ */
