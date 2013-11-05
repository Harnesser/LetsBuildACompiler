#! /usr/bin/env sh

for _file in ../test_data/conditionals_one_letter/*.txt; do
	echo "=============================================================="
	echo $_file
	expr_in=$(cat $_file)
	echo "INPUT: $expr_in"
	./main < $_file
done


