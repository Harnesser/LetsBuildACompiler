#! /usr/bin/env sh

	echo "=============================================================="
	./main > assembly.s

	cat assembly.s
	rm -f machinecode
	make -f Assembly.mk
	./machinecode
	res=$?
	if [ $res -gt 128 ]
	then
		echo "Negative"
		res=$(expr $res - 256)
	fi
	echo "Result: $res"


