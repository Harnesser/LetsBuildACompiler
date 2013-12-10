# ------------------------
runio: io
	./io

io: io.o 
	ld io.o -o io

io.o: io.s
	as io.s -o io.o

# --------------------------
clean:
	\rm -f io.o io

