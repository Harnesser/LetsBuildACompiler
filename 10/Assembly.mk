all: machinecode
	./machinecode

machinecode: assembly.o 
	ld assembly.o -o machinecode

assembly.o: assembly.s
	as -g assembly.s -o assembly.o

clean:
	@rm -f machinecode assembly.s assembly.o



