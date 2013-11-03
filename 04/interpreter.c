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

/* -------------------------------------------------------------------- */
int Expression(void);

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

int IsAddop(const char tok)
{
	if (tok=='-' || tok=='+' ) {
		return 1;
	}
	return 0;
}

int Term(void)
{
	int value;
	value = GetNum();
	while ( (Look=='*') || (Look=='/') ) {
		switch(Look) {
		case '*':
			Match('*');
			value *= GetNum();
			break;
		case '/':
			Match('/');
			value /= GetNum();
			break;
		}
	}
	return value;
}


int Expression(void)
{
	int value;
	if (IsAddop(Look)) {
		value = 0;
	} else {
		value = Term();
	}
	while (IsAddop(Look)) {
		switch(Look) {
		case '+':
			Match('+');
			value += Term();
			break;
		case '-':
			Match('-');
			value -= Term();
			break;
		}
	}
	return value;
}

/* -------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
	Init();
	while (Look != EOF) {
		printf("Res: %d\n", Expression() );
		if (Look != '\n' ) {
			Expected("Newline");
		} 
		Match('\n');
	}
}
