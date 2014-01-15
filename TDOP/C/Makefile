CPP=gcc
CFLAGS=-Wall -Wextra  
FILES=$(wildcard *.c)

all: test

test: main
	./test.py

main : $(FILES)
	$(CPP) $(CFLAGS) main.c -lm -I. -o main

clang: CPP=clang
clang: main

run:
	./main < stream_simple_add.txt

clean:
	rm -rf main	
