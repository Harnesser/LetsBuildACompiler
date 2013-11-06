// compiler cradle program
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const int TAB = '\t';
const int MAXMSG = 100;
const int MAXLBL = 8+1;

int lineno;

char Look;  /* lookahead character */

void GetChar(void)
{
	Look = getchar();
}

void Error(const char *msg)
{
	printf("\n");
	printf("Error: \"%s\" at line %d\n", msg, lineno);
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

int IsBoolean(char tok)
{
	char tokup;
	tokup = toupper(tok);
	if ( (tokup=='T') || (tokup=='F') ) {
		return 1;
	}
	return 0;
}

int IsOrOp(char tok)
{
	if ( (tok=='|') || (tok=='^') ) {
		return 1;
	}
	return 0;
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

int GetNum(void)
{
	char num;
	if (!IsDigit(Look)) {
		Expected("Integer");
	}
	num = Look;
	GetChar();
	return (int)(num - '0');
}

int GetBoolean(void)
{
	int truth;
	char tokup;
	if (!IsBoolean(Look)) {
		Expected("Boolean Literal");
	}
	tokup = toupper(Look);
	truth = (tokup == 'T');
	GetChar();
	return truth;
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
	lineno = 1;
	GetChar();
}

/* -------------------------------------------------------------------- */

void BoolFactor(void)
{
	if (!IsBoolean(Look)) {
		Expected("Boolean Literal");
	}
	if (GetBoolean()) {
		EmitLn("movl $-1,%eax");
	} else {
		EmitLn("movl $0,%eax");
	}
}

void NotFactor(void)
{
	if (Look=='!') {
		Match('!');
		BoolFactor();
		EmitLn("not %eax");
	} else {
		BoolFactor();
	}
}

void BoolTerm(void)
{
	NotFactor();
	while (Look=='&') {
		EmitLn("pushl %eax");
		Match('&');
		NotFactor();
		EmitLn("popl %edx");
		EmitLn("andl %edx, %eax");
	}
}

void BoolOr(void)
{
	Match('|');
	BoolTerm();
	EmitLn("popl %edx");
	EmitLn("orl %edx, %eax");
}

void BoolXor(void)
{
	Match('^');
	BoolTerm();
	EmitLn("popl %edx");
	EmitLn("xorl %edx, %eax");
}

void BoolExpression(void)
{
	BoolTerm();
	while (IsOrOp(Look)) {
		EmitLn("push %eax");
		switch(Look) {
		case '|': BoolOr(); break;
		case '^': BoolXor(); break;
		}
	}
}

/* -------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
	Init();
	while (Look != EOF) {
		while (Look=='\n') {
			Match('\n');
			lineno++;
		}
		BoolExpression();
		GetChar();
	}
}

