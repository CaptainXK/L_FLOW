#! /bin/bash
rm -rf test
if [ "$1" == "dgb" ]
then
	echo "gdb mode"
	g++ *.cpp -g -o test
else
	echo "none dgb mode"
	g++ *.cpp -o test
fi

chmod 777 test
./test
