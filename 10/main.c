// compiler cradle program
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef NMESSAGE
#define message(M, ...)
#else
#define message(M, ...) \
	Printable(pLook, Look); \
	printf("### " M , ##__VA_ARGS__); \
	printf(" Line %d col %d : Look '%s' \n", lineno, colno, pLook); \
	fflush(NULL);
#endif

const int MAXMSG = 100;
const int MAXLBL = 8+1;
const int MAXNAME = 25+1;
const int MAXOPER = 5;

int lineno;
int colno;
int labelno;

char Look;  /* lookahead character */
char pLook[26]; /* printable version of Look */
char label[9]; /* label for machine code  conditionals */
int ST[26];

void GetChar(void)
{
	Look = getchar();
	colno++;
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
int IsWhite(const char tok) { return isblank(tok); }

void SkipWhite(void)
{
	//message("Chomping whitespace...");
	while (IsWhite(Look)) {
		GetChar();
	}
	//message("Done chomping whitespace");
}

void Match(const char tok)
{
	char msg[MAXMSG];
	message("Looking to match \'%c\'", tok);
	NewLine();
	if (Look == tok) {
		GetChar();
		message("Matched");
	} else {
		snprintf(msg, MAXMSG, "\"%c\" Look=\"%c\"", tok, Look);
		Expected(msg);
	}
	SkipWhite();
}

void Printable(char *pline, char tok)
{
	switch(tok) {
	case '\n': strncpy(pLook, "NEWLINE", MAXNAME); break;
	case '\t': strncpy(pLook, "TAB", MAXNAME); break;
	case '\0': strncpy(pLook, "EOF", MAXNAME); break;
	default  : pLook[0] = tok; pLook[1] = '\0'; break;
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
	int i;
	lineno = 1;
	labelno = 0;
	colno = 0;
	GetChar();
	SkipWhite();
	for(i=0;i<26;i++) {
		ST[i] = 0;
	}
	message("Init Done");
}

/* -------------------------------------------------------------------- */

// forward declarations
void Expression(void);
void Block(void);

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
#include "conditional.c"


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
	NewLine();
	message("Top Declarations");
	while (Look != 'b') {
		switch(Look) {
		case 'v': Decl(); break;
		default:
			snprintf(msg, MAXMSG, "Unrecognised keyword \'%c\'", Look);
			
			Abort(msg);
			break;
		}
		NewLine();
	}
	message("  ");
	message("Top Declarations Done");
}

void Block(void)
{
	NewLine();
	message("Block");
	while ( (Look!='e') && (Look!='l') )  {
		switch (Look) {
		case 'i': DoIf(); break;
		case 'w': DoWhile(); break;
		default : Assignment(); break;
		}
		NewLine();
	}
	message("Endblock");
}

void Main(void)
{
	message("main");
	NewLine();
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
	NewLine();
	Match('p');
	message("Starting Program");
	Header();
	message("TopDecls");
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

