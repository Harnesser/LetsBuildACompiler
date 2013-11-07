test: main
	./test.py

main: main.c scanning.c
	gcc main.c -o main

clean:
	@rm -f main
	@make -f Assembly.mk clean

