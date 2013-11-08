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

void Clear(void) { EmitLn("movl $0, %eax"); }
void Negate(void) { EmitLn("neg %eax"); }
void Push(void) { EmitLn("pushl %eax"); }

void LoadConst(int val) {
	char code[MAXMSG];
	snprintf(code, MAXMSG, "movl $%d, %%eax", val);
	EmitLn(code);
}

void Undefined(char name)
{
	char msg[MAXMSG];
	snprintf(msg, MAXMSG, "Undefined identifier \'%c\'", name);
	Abort(msg);
}

void LoadVar(char name) {
	char code[MAXMSG];
	if (!InTable(name) ) {
		Undefined(name);
	}
	snprintf(code, MAXMSG, "movl $%c, %%edx", name);
	EmitLn(code);
	EmitLn("movl (%edx), %eax");
}

void PopAdd(void)
{
	EmitLn("popl %ebx");
	EmitLn("addl %ebx, %eax");
}

void PopSub(void)
{
	EmitLn("popl %ebx");
	EmitLn("subl %ebx, %eax");
	Negate();
}

void PopMul(void)
{
	EmitLn("popl %ebx");
	EmitLn("imul %ebx");
}

void PopDiv(void)
{
	EmitLn("movl %eax,%ebx");
	EmitLn("popl %eax");
	EmitLn("movl $0,%edx");
	EmitLn("divl %ebx");
}

void Store(char name)
{
	char code[MAXMSG];
	if (!InTable(name) ) {
		Undefined(name);
	}
	snprintf(code, MAXMSG, "movl $%c,%%edx", name);
	EmitLn(code);
	EmitLn("movl %eax,(%edx)\t\t# assignment");
}

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
