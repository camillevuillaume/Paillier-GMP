Paillier-GMP
============

# License 

Paillier-GMP is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Paillier-GMP.  If not, see <http://www.gnu.org/licenses/>.

## General Description

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

You need a system with /dev/urandom and GMP to run this program.

## Syntax for the built-in interpreter

Available commands:
```
paillier keygen [public key file name] [private key file name] [bit length]
paillier encrypt [output ciphertext file name] [input plain text file name] [public key file name]
paillier decrypt [output plaintext file name] [input ciphertext file name] [private key file name]
paillier homoadd [output ciphertext 3 file name] [input ciphertext 1 file name] [input ciphertext 2 file name] [public key file name]
paillier homomul [output ciphertext 2 file name] [input ciphertext 1 file name] [input constant file name] [public key file name]
```

For example
```
./paillier keygen pub2048 priv2048 2048
 echo 5 > m1
 echo 6 > m2
 ./paillier encrypt c1 m1 pub2048
 ./paillier encrypt c2 m2 pub2048
 ./paillier homoadd c3 c1 c2 pub2048
 ./paillier decrypt m3 c3 priv2048
 cat c3
```
Should yield 0xb=11=5+6.

## Building the program

Make options:
 - "make all" will build the documentation, the interpreter and the static library.
 - "make release" will build the interpreter.
 - "make doc" will build the documentation.
 - "make debug" will build the interpreter with debug symbols.

## Warning

There is basically no formatting check. If message/key formatting is incorrect, in the best case the program will crash.
The private key is stored unencrypted. Use the program at your own risk!

