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

void Alloc(char name)
 {
	int x;	
	char msg[MAXMSG];

	// check for dupes
	if (InTable(name)) {
		snprintf(msg, MAXMSG, "Dupe variable: %c\n", name);
		Abort(msg);
	} 
	x = name - 'A';
	ST[x] = 1;

	// write storage in .data sectoin
	printf("%c:\t .long ", name );
	if (Look=='=') {
		Match('=');
		if (Look=='-') {
			printf("-");
			Match('-');
		}
		printf("%d\n", GetNum());
	} else {
		printf("0\n");
	}
}
