#!/bin/bash
if [ ! -e pub4096.txt ] && [ ! -e priv4096.txt ]; then
	echo 'Generating keys. This might take a while.'
	../build/paillier keygen pub4096.txt priv4096.txt 4096
fi
echo "Homomorphic addition 3+4 using enc(3) and enc(4)."
echo 3 > m1.txt
echo 4 > m2.txt
../build/paillier encrypt c1.txt m1.txt pub4096.txt
../build/paillier encrypt c2.txt m2.txt pub4096.txt
../build/paillier homoadd c3.txt c1.txt c2.txt pub4096.txt
../build/paillier decrypt m3.txt c3.txt priv4096.txt
result=`cat m3.txt`
if [ "$result" == "7" ]; then
	echo "[OK] -> $result == 0x7"
else
	echo "[NG] -> $result !== 0x7"
fi
echo "Homomorphic multiplication 4x5 using enc(4) and 5."
echo 5 > m4.txt
../build/paillier homomul c5.txt c2.txt m4.txt pub4096.txt
../build/paillier decrypt m5.txt c5.txt priv4096.txt
result2=`cat m5.txt`
if [ "$result2" == "14" ]; then
	echo "[OK] -> $result2 == 0x14"
else
	echo "[NG] -> $result2 != 0x14"
fi
