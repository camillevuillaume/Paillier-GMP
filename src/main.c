/**
 * @file main.c
 *
 * @date Created on: Aug 25, 2012
 * @author Camille Vuillaume
 * @copyright Camille Vuillaume, 2012
 * @defgroup Interpreter Command interpreter for Paillier cryptosystem
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

/** @mainpage
 *
 * Paillier-GMP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Paillier-GMP.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section des General Description
 *
 * This program implements the Paillier cryptosystem with GMP for low-level functions (modular exponentiations, multiplications...).
 * It uses the following implementation tricks:
 * - Whenever possible (i.e. key generation and decryption) exponentiations are computed using the Chinese Remainder Theorem (CRT).
 * - The basis g is selected as 1+n, which allows faster encryption.
 * - The value n^{-1} mod 2^len is pre-calculated and stored in the private key, which allows fast calculations of divisions by n.
 *
 * The program includes:
 * - Memory allocation/free routines.
 * - Import and export of the public/private keys to files.
 * - A small command interpreter for generating keys, encrypting and decrypting text files (or stdin).
 *
 * From the project, you can:
 * - Compile a static library.
 * - Compile the interpreter.
 * - Compile the interpreter with debug symbols and debug messages.
 * - Compile the documentation with doxygen.
 *
 * @section req Requirements
 *
 * You need a system with /dev/urandom and GMP to run this program.
 *
 * @section syntx Syntax for the built-in interpreter
 *
 * Available commands:
 * - paillier keygen [public key file name] [private key file name] [bit length]
 * - paillier encrypt [output ciphertext file name] [input plain text file name] [public key file name]
 * - paillier decrypt [output plaintext file name] [input ciphertext file name] [private key file name]
 *
 * @section build Building the program
 *
 * Make options:
 * - "make all" will build the documentation, the interpreter and the static library.
 * - "make release" will build the interpreter.
 * - "make doc" will build the documentation.
 * - "make debug" will build the interpreter with debug symbols.
 *
 * @section warning Warning
 *
 * There is no formatting check. If message/key formatting is incorrect, in the best case the program will crash.
 * Also be warned that the private key is stored unencrypted. Use the program at your own risk!
 *
 */
#include <stdio.h>
#include <string.h>
#include "paillier.h"

/** Help message
 *
 * @ingroup Interpreter
 */
const char *hlp_message =
		"Syntax: paillier [options]\n"
		"options:\n"
		"  keygen [public_key_file] [private_key_file] [bit length]\n"
		"  encrypt [out_file] [in_file] [public_key_file]\n"
		"  decrypt [out_file] [in_file] [private_key_file]\n";

/** Main function
 *
 * @ingroup Interpreter
 * Run key generation, encryption or decryption.
 *
 * @param[in] argc number of arguments
 * @param[in] argv arguments
 * - keygen [public_key_file] [private_key_file] [bit length]
 * - encrypt [out_file] [in_file] [public_key_file]
 * - decrypt [out_file] [in_file] [private_key_file]
 */
int main(int argc, char *argv[]) {
	FILE *fp1, *fp2, *fp3;
	int bitlen;

	if(argc == 5 && strcmp(argv[1], "keygen")==0) {
		fp1 = fopen(argv[2], "w");
		fp2 = fopen(argv[3], "w");
		sscanf(argv[4], "%d", &bitlen);
		paillier_keygen_str(fp1, fp2, bitlen);
		fclose(fp1);
		fclose(fp2);
	}
	else if(argc == 5 && strcmp(argv[1], "encrypt")==0) {
		fp1 = fopen(argv[2], "w");
		fp2 = fopen(argv[3], "r");
		fp3 = fopen(argv[4], "r");
		paillier_encrypt_str(fp1, fp2, fp3);
		fclose(fp1);
		fclose(fp2);
		fclose(fp3);
	}
	else if(argc == 5 && strcmp(argv[1], "decrypt")==0) {
		fp1 = fopen(argv[2], "w");
		fp2 = fopen(argv[3], "r");
		fp3 = fopen(argv[4], "r");
		paillier_decrypt_str(fp1, fp2, fp3);
		fclose(fp1);
		fclose(fp2);
		fclose(fp3);
	}
	else {
		fputs(hlp_message, stderr);
	}
	return 0;
}
