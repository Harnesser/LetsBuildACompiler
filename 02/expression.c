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
	return isalnum(tok);
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
void Expression(void)
{
	char msg[MAXMSG];
	snprintf(msg, MAXMSG, "MOVE #%c, D0", GetNum() );
	EmitLn(msg);
}

/* -------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
	Init();
	Expression();
}



