char asm_header[] = "#PURPOSE:  KISS Program \n\
.section .data \
\n";

char asm_prolog[] = "\
.section .text \n \
.globl _start \n \
\n \
_start: \n \
";

char asm_epilog[] = "\
movl %eax,%ebx \n \
movl $1, %eax      # sys_exit \n \
int $0x80           # call \n \
";

void Header(void) { printf("%s\n", asm_header); }
void Prolog(void) { printf("%s\n", asm_prolog); }
void Epilog(void) { printf("%s\n", asm_epilog); }

void Alloc(char name) {
	printf("%c:\t .long ", name );
	if (Look=='=') {
		Match('=');
		printf("%d\n", GetNum());
	} else {
		printf("0\n");
	}
}
