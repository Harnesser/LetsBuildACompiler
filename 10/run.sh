#! /usr/bin/env sh

PROGRAM=../test_data/kiss/$1.txt
EXPECT=$2

echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - - "
echo "Test: $PROGRAM"
echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - - "
rm -f machinecode main
mkdir -p asm

# Build
make main
./main <${PROGRAM} >assembly.s
cp assembly.s asm/$1.asm
cat assembly.s
make -f Assembly.mk

# Test
./machinecode
res=$?
if [ $res -gt 128 ]; then
	echo "Negative"
	res=$(expr $res - 256)
fi

echo "Result: $res"
if [ $res -eq $EXPECT ]; then 
	echo "Passed"
	exit 0
fi

echo "FAILED"
exit 1


