/**
 * @file main.c
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

#include <stdio.h>
#include "paillier.h"

/** Help message
 *
 */
const char *hlp_message =
		"Syntax: paillier [options]\n"
		"options:\n"
		"  keygen [public_key_file] [private_key_file] [bit length]\n"
		"  encrypt [out_file] [in_file] [public_key_file]\n"
		"  decrypt [out_file] [in_file] [private_key_file]\n";

/** Main function
 *
 * Run key generation, encryption or decryption.
 *
 * @param[in] argc number of arguments
 * @param[in] argv
 * keygen [public_key_file] [private_key_file] [bit length]
 * encrypt [out_file] [in_file] [public_key_file]
 * decrypt [out_file] [in_file] [private_key_file]
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
