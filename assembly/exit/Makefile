all: exit
	./exit

exit: exit.o 
	ld exit.o -o exit

exit.o: exit.s
	as exit.s -o exit.o

clean:
	\rm -f exit.o exit

