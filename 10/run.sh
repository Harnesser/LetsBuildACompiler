#! /usr/bin/env sh

PROGRAM=../test_data/kiss/$1.txt
EXPECT=$2

echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - - "
echo "Test: $PROGRAM"
echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - - "
rm -f machinecode main

make main
./main <${PROGRAM} >assembly.s
cat assembly.s
make -f Assembly.mk
./machinecode
res=$?
if [ $res -gt 128 ]
then
	echo "Negative"
	res=$(expr $res - 256)
fi

echo "Result: $res"
if [ $res -eq $EXPECT ] 
then
	echo "Passed"
	exit 0
fi

echo "FAILED"
exit 1


