// compiler cradle program
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const int TAB = '\t';
const int MAXMSG = 100;

char Look;  /* lookahead character */

void GetChar(void)
{
	Look = getchar();
}

void Error(const char *msg)
{
	printf("\n");
	printf("Error: %s.\n", msg);
}

void Abort(const char *msg)
{
	Error(msg);
	exit(1);
}

void Expected(const char *msg)
{
	char msgl[MAXMSG];
	strncpy(msgl, msg, MAXMSG);
	strncat(msgl, " Expected", MAXMSG);
	Abort(msgl);
}

void Match(const char tok)
{
	char msg[MAXMSG];
	if (Look == tok) {
		GetChar();
	} else {
		snprintf(msg, MAXMSG, "\"%c\"", tok);
		Expected(msg);
	}
}

int IsAlpha(const char tok)
{
	return isalpha(tok);
}

int IsDigit(const char tok)
{
	return isdigit(tok);
}

char GetName(void)
{
	char name;
	if (!IsAlpha(Look)) {
		Expected("Name");
	}
	name = toupper(Look);
	GetChar();
	return name;
}

char GetNum(void)
{
	char num;
	if (!IsDigit(Look)) {
		Expected("Integer");
	}
	num = Look;
	GetChar();
	return num;
}

void Emit(const char *msg)
{
	printf("\t%s", msg);
}

void EmitLn(const char *msg)
{
	Emit(msg);
	printf("\n");
}

void Init(void)
{
	GetChar();
}

/* Expression */
void Expression(void);

void Ident(void)
{
	char name;
	char str[MAXMSG];

	name = GetName();
	if (Look=='(') {
		/* function call */
		Match('(');
		Match(')'); // empty arg list for now
		snprintf(str, MAXMSG, "bsr %c", name);
	} else {
		/* variable */
		snprintf(str, MAXMSG, "movl $%c, %%edx", name);
		EmitLn(str);
		EmitLn("movl (%edx), %eax");
	}	
}

void Factor(void)
{
	char str[MAXMSG];
	if (Look=='(') {
		Match('(');
		Expression();
		Match(')');
	} else if (IsAlpha(Look)) {
		Ident();
	} else {
		snprintf(str, MAXMSG, "movl $%c,%%eax", GetNum() );
		EmitLn(str);
	}
}

void Multiply(void)
{
	Match('*');
	Factor();
	EmitLn("popl %ebx");
	EmitLn("imul %ebx");
}

void Divide(void)
{
	Match('/');
	Factor();
	EmitLn("movl %eax,%ebx");
	EmitLn("popl %eax");
	EmitLn("movl $0,%edx");
	EmitLn("divl %ebx");
}

void Term(void)
{
	Factor();
	while (Look=='*' || Look=='/') {
		EmitLn("pushl %eax");
		switch (Look) {
		case '*' : 
			Multiply();
			break;
		case '/' :
			Divide();
			break;
		default:
			Expected("Mulop");
			break;
		}
	}
}

void Add(void)
{
	Match('+');
	Term();
	EmitLn("popl %ebx");
	EmitLn("addl %ebx, %eax");
}

void Subtract(void)
{
	Match('-');
	Term();
	EmitLn("popl %ebx");
	EmitLn("subl %ebx, %eax");
	EmitLn("neg %eax");
}

int IsAddop(const char tok)
{
	if (tok=='-' || tok=='+' ) {
		return 1;
	}
	return 0;
}

void Expression(void)
{
	if (IsAddop(Look)) {
		EmitLn("clr %eax");
	} else {
		Term();
	}

	while (IsAddop(Look)) {
		EmitLn("pushl %eax");
		switch (Look) {
		case '+':
			Add();
			break;
		case '-':
			Subtract();
			break;
		default:
			Expected("Addop");
			break;
		}
	}
}

void Assignment(void)
{
	char name;
	char str[MAXMSG];
	name = GetName();
	Match('=');
	Expression();
	snprintf(str, MAXMSG, "movl $%c,%%edx", name);
	EmitLn(str);
	EmitLn("movl %eax,(%edx)");
}
/* -------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
	Init();
	while (Look != EOF) {
		Assignment();
		if (Look != '\n' ) {
			Expected("Newline");
		} 
		Match('\n');
	}
}
