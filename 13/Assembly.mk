all: machinecode
	./machinecode

machinecode: assembly.o 
	ld -m elf_i386 assembly.o -o machinecode

assembly.o: assembly.s
	as --32 -g assembly.s -I../asm -o assembly.o

clean:
	@rm -f machinecode assembly.s assembly.o



