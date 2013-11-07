// compiler cradle program
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const int TAB = '\t';
const int MAXMSG = 100;
const int MAXLBL = 8+1;
const int MAXNAME = 25;

int lineno;
int colno;
int labelno;

char Name[25];  /* identifier name */
char Num[25];   /* number string */
char Token[25]; /* scanned token */

char Look;  /* lookahead character */
char label[9]; /* label for machine code  conditionals */


void GetChar(void)
{
	Look = getchar();
	if (Look==EOF) {
		printf("# Done\n");
		exit(0);
	}
	colno++;
}

void Fin(void)
{
	if (Look=='\n') {
		GetChar();
		colno = 0;
		lineno++;
	}
}

void Error(const char *msg)
{
	printf("\n");
	printf("Error: \"%s\" at line %d column %d\n", msg, lineno, colno);
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

int IsAlNum(const char tok)
{
	return isalnum(tok);
}

int IsWhite(const char tok)
{
	return isblank(tok);
}

void SkipWhite(void)
{
	while (IsWhite(Look)) {
		GetChar();
	}
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
	labelno = 0;
	colno = 0;
	GetChar();
}

/* -------------------------------------------------------------------- */
// forward declarations

// stuff
#include "scanning.c"

/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
	Init();
	while (Look != EOF) {
		Scan();
		printf("Scanned: %s\n", Token);
	}
}

