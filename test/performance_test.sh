#!/bin/bash

echo 3 > m1.txt

if [ ! -e pub1024.txt ] && [ ! -e priv1024.txt ]; then
	echo 'Generating keys. This might take a while.'
	../build/paillier keygen pub1024.txt priv1024.txt 1024
fi
echo "encryption performance, 1024 bits"
time ../build/paillier encrypt c1.txt m1.txt pub1024.txt
echo "decryption performance, 1024 bits"
time ../build/paillier decrypt m2.txt c1.txt priv1024.txt

if [ ! -e pub2048.txt ] && [ ! -e priv2048.txt ]; then
	echo 'Generating keys. This might take a while.'
	../build/paillier keygen pub2048.txt priv2048.txt 2048
fi
echo "encryption performance, 2048 bits"
time ../build/paillier encrypt c1.txt m1.txt pub2048.txt
echo "decryption performance, 2048 bits"
time ../build/paillier decrypt m2.txt c1.txt priv2048.txt

if [ ! -e pub4096.txt ] && [ ! -e priv4096.txt ]; then
	echo 'Generating keys. This might take a while.'
	../build/paillier keygen pub4096.txt priv4096.txt 4096
fi
echo "encryption performance, 4096 bits"
time ../build/paillier encrypt c1.txt m1.txt pub4096.txt
echo "decryption performance, 4096 bits"
time ../build/paillier decrypt m2.txt c1.txt priv4096.txt
