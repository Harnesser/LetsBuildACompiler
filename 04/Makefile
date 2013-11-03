test: interpreter
	./test.sh

expression: interpreter.c
	gcc interpreter.c -o interpreter

clean:
	@rm -f interpreter
	@make -f Assembly.mk clean

