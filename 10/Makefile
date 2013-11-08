test: main
	@echo "\033[32m Testing \033[0m"
	./test.py

main: main.c scanning.c
	@echo "\033[32m Compiling \033[0m"
	gcc main.c -o main

clean:
	@rm -f main
	@make -f Assembly.mk clean

