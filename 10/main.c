// compiler cradle program
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifdef NMESSAGE
#define message(M, ...)
#else
#define message(M, ...) \
	printf("### " M , ##__VA_ARGS__); \
	printf(" Line %d col %d : Look '%c' \n", lineno, colno, Look);
#endif

const int MAXMSG = 100;
const int MAXLBL = 8+1;
const int MAXNAME = 25+1;
const int MAXOPER = 5;

int lineno;
int colno;
int labelno;

char Look;  /* lookahead character */
char label[9]; /* label for machine code  conditionals */
int ST[26];

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
	int i;
	lineno = 1;
	labelno = 0;
	colno = 0;
	GetChar();
	for(i=0;i<26;i++) {
		ST[i] = 0;
	}
}

/* -------------------------------------------------------------------- */

// forward declarations
void Expression(void);

int InTable(char c)
{
	int i;
	i = c - 'A';
	return ST[i];
}

// stuff
#include "assembly.c"
#include "scanning.c"
#include "boolean.c"
#include "arithmetic.c"
//#include "conditional.c"


void Decl(void)
{
	char name;
	Match('v');
	Alloc(GetName());
	while (Look==',') {
		GetChar();
		Alloc(GetName());
	}
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

void Block(void)
{
	message("Block");
	while (Look!='e') {
		Assignment();
	}
	message("Endblock");
}

void Main(void)
{
	message("main");
	Match('b');
	Prolog();
	Block();
	Match('e');
	Epilog();
	message("endmain");
}

void Prog(void)
{
	message("Program");
	Match('p');
	Header();
	TopDecls();
	Main();
	Match('.');
	message("Endprogram");
}


/* -------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
	Init();
	Prog();
}

