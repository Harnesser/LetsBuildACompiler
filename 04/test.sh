#! /usr/bin/env sh

for _file in ../test_data/expressions/*.txt; do
	echo "=============================================================="
	echo $_file
	expr_in=$(cat $_file)
	echo "INPUT: $expr_in"
	./interpreter < $_file
done


