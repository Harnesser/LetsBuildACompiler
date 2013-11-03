#! /usr/bin/env sh

for _file in data/*.txt; do
	echo "=============================================================="
	echo $_file
	expr_in=$(cat $_file)
	echo "INPUT: $expr_in"
	./expression < $_file > assembly.s
	cat assembly.s
	rm -f machinecode
	make -f Assembly.mk
	./machinecode
	echo "Result: $?"
done


