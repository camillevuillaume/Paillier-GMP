#!/bin/bash
if [ ! -e pub4096 ] && [ ! -e priv4096 ]; then
	echo 'Generating keys. This might take a while.'
	./paillier keygen pub4096 priv4096 4096
fi
echo "Homomorphic addition 3+4 using enc(3) and enc(4)."
echo 3 > m1
echo 4 > m2
./paillier encrypt c1 m1 pub4096
./paillier encrypt c2 m2 pub4096
./paillier homoadd c3 c1 c2 pub4096
./paillier decrypt m3 c3 priv4096
result=`cat m3`
if [ "$result" == "7" ]; then
	echo "[OK] -> $result == 7"
else
	echo "[NG] -> $result !== 7"
fi
echo "Homomorphic multiplication 4x5 using enc(4) and 5."
echo 5 > m4
./paillier homomul c5 c2 m4 pub4096
./paillier decrypt m5 c5 priv4096
result2=`cat m5`
if [ "$result2" == "14" ]; then
	echo "[OK] -> $result == 14"
else
	echo "[NG] -> $result != 14"
fi
