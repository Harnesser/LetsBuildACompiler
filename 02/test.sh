#! /usr/bin/env sh

for _file in ../test_data/*.txt; do
	echo "=============================================================="
	echo $_file
	expr_in=$(cat $_file)
	echo "INPUT: $expr_in"
	./expression < $_file > assembly.s
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
done


