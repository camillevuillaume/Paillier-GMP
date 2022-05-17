Paillier-GMP
============

## Overview

The Paillier cryptosystem is a public-key cryptosystem with a very interesting property: given the encryption of two messages m1 and m2, anybody can compute the encryption of m1+m2! There are multiple applications, including for instance secure electronic voting, or private information retrieval, where one can query a database without the database knowing which element has been retrieved, and far more.
While there are other cryptosystems like pairing-based cryptography where it is also possible to compute one addition on top of additions, and even fully homomorphic cryptosystems, the Paillier cryptosystem has the advantage of being relatively simple, efficient and is based on well-understood mathematics. Furthermore, it is sufficient for many applications.

The motivation of this program is to implement the Paillier cryptosystem in C using the GMP library (GNU Multiple Precision Arithmetic Library) for low-level functions. The cryptosystem is provided as a library, and a small command interpreter is also available to execute various tasks like key generation, encryption, decryption, homomorphic operations from the command line. The program was developed from a Linux machine and has not been tested on a different environment.

## Detailed Description

This program implements the Paillier cryptosystem with GMP for low-level functions (modular exponentiations, multiplications...).
It uses the following implementation tricks:
 - Whenever possible (i.e. key generation and decryption) exponentiations are computed using the Chinese Remainder Theorem (CRT).
 - When the program is compiled with the thread option, CRT exponentiation uses two threads, one per exponentiation.
 - The basis g is selected as 1+n, which allows faster encryption.
 - The value n^{-1} mod 2^len is pre-calculated and stored in the private key, which allows fast calculations of divisions by n.

 The program includes:
 - Memory allocation/free routines for public/private keys.
 - Import and export of the public/private keys to files.
 - Key generation, encryption and decryption.
 - A small command interpreter for generating keys, encrypting and decrypting text files (or stdin).

Using the provided makefile, you can:
 - Compile a static library.
 - Compile the interpreter.
 - Compile the interpreter with debug symbols and debug messages.
 - Compile the documentation with doxygen.

## Requirements

You need a system with `/dev/urandom` and GMP to run this program.
On system with apt as package manager like Ubuntu for instance, GMP can be installed with the following command.
```
sudo apt install libgmp-dev
```

## Syntax for the built-in interpreter

Available commands:
```
paillier keygen [public key file name] [private key file name] [bit length]
```

Generate two files, one storing the public key, the other the private key, based on the specified keylength. Example: `/paillier keygen pub2048 priv2048 2048` will generate the 2048-bit public and private keys and store them in two files.

```
paillier encrypt [output ciphertext file name] [input plain text file name] [public key file name]
```

From the public key and plaintext files specified in the input, the program calculates the ciphertext via encryption and stores it in a file. Example: `./paillier encrypt c1 m1 pub2048` will encrypt the plaintext from the file `m1` and store it in file `c1`, using the public key from file `pub2048`.


```
paillier decrypt [output plaintext file name] [input ciphertext file name] [private key file name]
```

From the private key and ciphertext files specified in the input, the program calculates the plaintext via decryption and stores it in a file. Example: `./paillier decrypt m2 c1 priv2048` will decrypt the ciphertext from the file `c1` and store it in file `m2`, using the private key from file `priv2048`.


```
paillier homoadd [output ciphertext 3 file name] [input ciphertext 1 file name] [input ciphertext 2 file name] [public key file name]
```

From two ciphertext and one public key files, the program homomorphically add the two input ciphertexts and stores the resulting ciphertext in a new file. Example: `./paillier homoadd c3 c2 c1 pub2048` will add the ciphertexts from the files `c1` and `c2` and store it in file `c3`, using the public key from file `pub2048`.

```
paillier homomul [output ciphertext 2 file name] [input ciphertext 1 file name] [input constant file name] [public key file name]
```

From a ciphertext, constant and public key files, the program homomorphically multiplies the constant and ciphertext and stores the resulting ciphertext in a new file. Example: `./paillier homomul c2 ct c1 pub2048` will multiply the ciphertext from the files `c1` with the constant stored in file `ct` and store it in file `c2`, using the public key from file `pub2048`.


Here is an example of a sequence of interpreter command executions.

```
./paillier keygen pub2048 priv2048 2048 ;
 echo 5 > m1 ;
 echo 6 > m2 ;
 ./paillier encrypt c1 m1 pub2048 ;
 ./paillier encrypt c2 m2 pub2048 ;
 ./paillier homoadd c3 c1 c2 pub2048 ;
 ./paillier decrypt m3 c3 priv2048 ;
 cat m3
```

Should yield 0xb=11=5+6.

## Building the program

Make options:
 - "make all" will build the documentation, the interpreter and the static library.
 - "make release" will build the interpreter (which includes all object files, but does not build the static library).
 - "make lib" will build the static library, but not the interpreter.
 - "make doc" will build the documentation.
 - "make debug" will build the interpreter with debug symbols.

## Warning

There is basically no formatting check. If message/key formatting is incorrect, in the best case the program will crash.
The private key is stored unencrypted. Use the program at your own risk!

## License 

Paillier-GMP is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Paillier-GMP.  If not, see <http://www.gnu.org/licenses/>.
