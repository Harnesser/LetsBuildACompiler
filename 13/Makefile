
SOURCES=$(wildcard src/*.c)
CFLAGS=-g

test: main
	@echo "\033[32m Testing \033[0m"
	./test.py

strict: CFLAGS+= -Wall -Wextra -Werror
strict: clean main
pedantic: CFLAGS+= -Wpedantic
pedantic: strict

main: $(SOURCES)
	@echo "\033[32m Compiling \033[0m"
	gcc $(CFLAGS) src/main.c -I./src -o main

clean:
	@rm -f main
	@make -f Assembly.mk clean

