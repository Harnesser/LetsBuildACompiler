COMPILER=gcc
SOURCES=$(wildcard src/*.c src/compiler/*.c)
LIBLCTHW=$(wildcard src/lcthw/*.c)
LIBLCTHW_H=$(wildcard src/lcthw/*.h)
TGT=build/liblcthw.a

CFLAGS=-g

test: main
	@echo "\033[32m Testing \033[0m"
	./test.py

strict: CFLAGS+= -Wall -Wextra -Werror
strict: clean main
pedantic: CFLAGS+= -Wpedantic
pedantic: strict

main: $(TGT)
main: $(SOURCES)
	@echo "\033[32m Compiling \033[0m"
	$(COMPILER) $(CFLAGS) src/main.c -I./src -L./build -llcthw -o main

clang: COMPILER=clang
clang: main

$(TGT): $(LIBLCTHW) $(LIBLCTHW_H)
	$(COMPILER) $(CFLAGS) -c -I./src $(LIBLCTHW)
	ar rcs $(TGT) *.o
	rm *.o

valgrind: 
	valgrind --leak-check=full \
		./main < ../test_data/kiss_procedures/procedure_param_list.txt
	
clean:
	@rm -f main
	@rm -f *.o
	@make -f Assembly.mk clean

