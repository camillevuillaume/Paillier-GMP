/**
 * @file paillier.h
 *
 * @date 		Created on: Aug 25, 2012
 * @author 		Camille Vuillaume
 * @copyright 	Camille Vuillaume, 2012
 * @defgroup 	Paillier Paillier cryptosystem
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
 */

#ifndef PAILLIER_H_
#define PAILLIER_H_

#include <stdio.h>
#include <gmp.h>

/** Private key
 *
 * @ingroup Paillier
 *
 * In addition to the usual private key elements, the structure contains:
 * - CRT parameters for accelerating exponentiations during decryption
 * - The modular inverse n^{-1} mod 2^len for accelerating the calculation of L
 */
typedef struct {
	mp_bitcnt_t len; /**< bit length of n */
	mpz_t lambda;		/**< least common multiple of p and q */
	mpz_t mu;			/**< Modular inverse */
	mpz_t p2;			/**< square of prime number p */
	mpz_t q2;			/**< square of prime number q */
	mpz_t p2invq2;		/**< CRT parameter p^{-2} mod q^2 */
	mpz_t ninv;			/**< modular inverse n^{-1} mod 2^len */
	mpz_t n;			/**< n=p*q */
} paillier_private_key;

/** Public key
 *
 * @ingroup Paillier
 *
 * The generator is 1+n. This is fine in view of security because Class[g,n] is random self-reducible over g,
 * therefore the security of the cryptosystem does not depend on the choice of g.
 */
typedef struct {
	mp_bitcnt_t len; /**< bit length of n */
	mpz_t n; 			/**< modulus n */
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


/** Output public key to stdio stream
 *
 * @ingroup Paillier
 * @param[out] fp output stream
 * @param[in] pub input public key
 */
int paillier_public_out_str(FILE *fp, paillier_public_key *pub);

/** Output private key to stdio stream
 *
 * @ingroup Paillier
 * @param[out] fp output stream
 * @param[in] priv input private key
 */
int paillier_private_out_str(FILE *fp, paillier_private_key *priv);

/** Input public key from stdio stream
 *
 * @ingroup Paillier
 * @param[out] pub output public key
 * @param[in] fp input stream
 */
int paillier_public_in_str(paillier_public_key *pub, FILE *fp);

/** Input private key from stdio stream
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
 * @param[in] len input bit length of public modulus
 * @return 0 if no error
 */
int paillier_keygen(
		paillier_public_key *pub,
		paillier_private_key *priv,
		mp_bitcnt_t len);


/** Key generation to stdio stream
 *
 * @ingroup Paillier
 * @param[out] public_key output stream for public key
 * @param[out] private_key output stream for private key
 * @param[in] len input bit length of public modulus
 * @return 0 if no error
 */
int paillier_keygen_str(
		FILE *public_key,
		FILE *private_key,
		int len);

/** Encrypt
 *
 * @ingroup Paillier
 * @param[out] ciphertext output ciphertext c=g^m*r^n mod n^2
 * @param[in] plaintext input plaintext m
 * @param[in] pub input public key
 * @return 0 if no error
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
 * @return 0 if no error
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
 * @return 0 if no error
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
 * @return 0 if no error
 */
int paillier_decrypt_str(
		FILE *ciphertext,
		FILE *plaintext,
		FILE *private_key);

/** Homomorphically add two plaintexts
 *
 * @ingroup Paillier
 * @param[out] ciphertext3 output ciphertext corresponding to the homomorphic addition of the two plaintexts
 * @param[in] ciphertext1 input first ciphertext corresponding to a plaintext to be homomorphically added
 * @param[in] ciphertext2 input second ciphertext corresponding to a plaintext to be homomorphically added
 * @param[in] pub input public key
 * @return 0 if no error
 */
int paillier_homomorphic_add(
		mpz_t ciphertext3,
		mpz_t ciphertext1,
		mpz_t ciphertext2,
		paillier_public_key *pub);

/** Homomorphically add two plaintexts from stdio stream
 *
 * @ingroup Paillier
 * @param[out] ciphertext3 output stream for result of homomorphic addition
 * @param[in] ciphertext1 input stream for first ciphertext c1
 * @param[in] ciphertext2 input stream for second ciphertext c2
 * @param[in] public_key input stream for public key
 * @return 0 if no error
 */
int paillier_homomorphic_add_str(
		FILE *ciphertext3,
		FILE *ciphertext1,
		FILE *ciphertext2,
		FILE *public_key);

/** Homomorphically multiply a plaintext with a constant
 *
 * @ingroup Paillier
 * @param[out] ciphertext2 output ciphertext corresponding to the homomorphic multiplication of the plaintext with the constant
 * @param[in] ciphertext1 input ciphertext corresponding to a plaintext to be homomorphically multiplied
 * @param[in] constant input constant to be homomorphically multiplied
 * @param[in] pub input public key
 * @return 0 if no error
 */
int paillier_homomorphic_multc(
		mpz_t ciphertext2,
		mpz_t ciphertext1,
		mpz_t constant,
		paillier_public_key *pub);


/** Homomorphically multiply a plaintext with a constant from stdio stream
 *
 * @ingroup Paillier
 * @param[out] ciphertext2 output stream for result of homomorphic multiplication
 * @param[in] ciphertext1 input stream for ciphertext c
 * @param[in] constant input stream for constant k
 * @param[in] public_key input stream for public key
 * @return 0 if no error
 */
int paillier_homomorphic_multc_str(
		FILE *ciphertext2,
		FILE *ciphertext1,
		FILE *plaintext,
		FILE *public_key);

#endif /* PAILLIER_H_ */
