// compiler cradle program
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const int MAXMSG = 100;
const int MAXLBL = 8+1;
const int MAXNAME = 25+1;
const int MAXOPER = 5;

int lineno;
int colno;
int labelno;

char Look;  /* lookahead character */
char label[9]; /* label for machine code  conditionals */


void GetChar(void)
{
	Look = getchar();
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
	printf("Error: %s at line %d column %d\n", msg, lineno, colno);
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

int IsAlpha(const char tok) { return isalpha(tok); }
int IsDigit(const char tok) { return isdigit(tok); }
int IsAlNum(const char tok) { return isalnum(tok); }
int IsWhite(const char tok) { return isspace(tok); }

void SkipWhite(void)
{
	while (IsWhite(Look)) {
		GetChar();
	}
}

void Match(const char tok)
{
	char msg[MAXMSG];
	if (Look == tok) {
		GetChar();
	} else {
		snprintf(msg, MAXMSG, "\"%c\" Look=\"%c\"", tok, Look);
		Expected(msg);
	}
	SkipWhite();
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
//#include "boolean.c"
//#include "arithmetic.c"
//#include "conditional.c"
void Header(void)
{
}

void Prolog(void)
{
}

void Epilog(void)
{
}

void Decl(void)
{
	Match('v');
	GetChar();
}

void TopDecls(void)
{
	char msg[MAXMSG];
	while (Look != 'b') {
		switch(Look) {
		case 'v': Decl(); break;
		default:
			snprintf(msg, MAXMSG, "Unrecognised keyword \'%c\'", Look);
			break;
		}
	}
}

void Main(void)
{
	Match('b');
	Prolog();
	Match('e');
	Epilog();
}

void Prog(void)
{
	Match('p');
	Header();
	TopDecls();
	Main();
	Match('.');
}


/* -------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
	Init();
	Prog();
}

