#!/bin/bash

echo 3 > m1
loops=10

if [ ! -e pub1024 ] && [ ! -e priv1024 ]; then
	echo 'Generating keys. This might take a while.'
	./paillier keygen pub1024 priv1024 1024
fi
echo "encryption performance, 1024 bits"
time ./paillier encrypt c1 m1 pub1024
echo "decryption performance, 1024 bits"
time ./paillier decrypt m2 c1 priv1024

if [ ! -e pub2048 ] && [ ! -e priv2048 ]; then
	echo 'Generating keys. This might take a while.'
	./paillier keygen pub2048 priv2048 2048
fi
echo "encryption performance, 2048 bits"
time ./paillier encrypt c1 m1 pub2048
echo "decryption performance, 2048 bits"
time ./paillier decrypt m2 c1 priv2048

if [ ! -e pub4096 ] && [ ! -e priv4096 ]; then
	echo 'Generating keys. This might take a while.'
	./paillier keygen pub4096 priv4096 4096
fi
echo "encryption performance, 4096 bits"
time ./paillier encrypt c1 m1 pub4096
echo "decryption performance, 4096 bits"
time ./paillier decrypt m2 c1 priv4096
