test: main
	./test.py

expression: main.c boolean.c arithmetic.c conditional.c
	gcc main.c -o main

clean:
	@rm -f main
	@make -f Assembly.mk clean

