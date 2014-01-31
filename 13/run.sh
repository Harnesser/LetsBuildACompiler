#! /usr/bin/env sh

TEST=$(basename --suffix=.txt $1 )
PROGRAM=../test_data/kiss_procedures/${TEST}.txt
EXPECT=$2
INPUT=$3

echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - - "
echo "Test: $PROGRAM"
echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - - "
echo " Input '$INPUT'"

rm -f machinecode main
mkdir -p listings

# Build
make main
./main <${PROGRAM} >assembly.s
cp assembly.s listings/$1.asm

mkdir -p build
rm -r build/*
mv assembly.s build
cd build
make -f ../Assembly.mk machinecode

# Test
echo $INPUT | ./machinecode
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
