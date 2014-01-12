CPP=gcc
CFLAGS=-Wall -Wextra 
FILES=$(wildcard *.c)

all: main run

main : $(FILES)
	$(CPP) $(CFLAGS) main.c -I. -o main

clang: CPP=clang
clang: main

run:
	./main < stream.txt

clean:
	rm -rf main	
