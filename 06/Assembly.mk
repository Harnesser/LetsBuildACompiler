all: machinecode
	./machinecode

machinecode: cradle.o 
	ld cradle.o -o machinecode

cradle.o: cradle.s assembly.s
	as -g cradle.s -o cradle.o

clean:
	@rm -f cradle.o machinecode assembly.s



