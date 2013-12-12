# ------------------------
runmain: main
	./main

main: main.o 
	ld main.o -o main

main.o: main.s read_integer.s
	as -g main.s -o main.o

# --------------------------
list:
	objdump -d main.o

gdb: main
	gdb -tui main --command=gdb.setup

# --------------------------
clean:
	\rm -f main.o main

