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
	int value;
	value = 0;
	if (!IsDigit(Look)) {
		Expected("Integer");
	}
	while (IsDigit(Look)) {
		value = (10*value) + (int)(Look-'0');
		GetChar();
	}
	return value;
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


int IsAddop(const char tok)
{
	if (tok=='-' || tok=='+' ) {
		return 1;
	}
	return 0;
}

int Factor(void)
{
	int factor;
	if (Look=='(') {
		Match('(');
		factor = Expression();
		Match(')');
	} else {
		factor = GetNum();
	}
	return factor;
}

int Term(void)
{
	int value;
	value = Factor();
	while ( (Look=='*') || (Look=='/') ) {
		switch(Look) {
		case '*':
			Match('*');
			value *= Factor();
			break;
		case '/':
			Match('/');
			value /= Factor();
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
