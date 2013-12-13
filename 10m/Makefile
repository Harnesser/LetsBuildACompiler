
SOURCES=$(wildcard src/*.c)

test: main
	@echo "\033[32m Testing \033[0m"
	./test.py

main: $(SOURCES)
	@echo "\033[32m Compiling \033[0m"
	gcc -g src/main.c -I./src -o main

clean:
	@rm -f main
	@make -f Assembly.mk clean

