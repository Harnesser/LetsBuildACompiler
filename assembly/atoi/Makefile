# ------------------------
runmain: main
	./main

main: main.o 
	ld main.o -o main

main.o: main.s read_integer.s
	as main.s -o main.o

# --------------------------
clean:
	\rm -f main.o main

