// compiler cradle program
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifdef NMESSAGE
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
const int MAXENTRY = 100; /* max # of symbols in the table */

int lineno;
int colno;
int labelno;

char Look;  /* lookahead character */
char pLook[26]; /* printable version of Look */
char label[9]; /* label for machine code  conditionals */

typedef enum { T_OTHER=0,
               T_IF, T_ELSE, T_ENDIF, T_END, T_BEGIN, T_VAR, T_WHILE, T_ENDWHILE,
               T_READ, T_WRITE,	
               T_PROGRAM_END,
               T_IDENT, T_NUMBER, T_OPER } e_token;

e_token TokenId;
char Token[26]; /* scanned token */
int Value; 

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
int IsWhite(const char tok) { return isspace(tok); }

void SkipWhite(void)
{
	//message("Chomping whitespace...");
	while (IsWhite(Look)) {
		if (Look == '\n') {
			lineno++;
			colno = 0;
		}
		GetChar();
	}
	//message("Done chomping whitespace");
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

/* -------------------------------------------------------------------- */

// forward declarations
void Expression(void);
void Block(void);

// stuff
#include "symboltable.c"
#include "scanning.c"
#include "assembly.c"
#include "boolean.c"
#include "arithmetic.c"
#include "conditional.c"
#include "io.c"

void Decl(void)
{
	char name[MAXNAME];
	message("Declaration");
	MatchString("VAR");
	strncpy(name, Token, MAXNAME);
	Alloc(name);
	//message("## [][][][][][][][][][] Token: \"%s\"", Token);
	while (Token[0]==',') {
		message("follow-on declaration");
		MatchString(",");
		strncpy(name, Token, MAXNAME);
		Alloc(name);
	}
	message("End of declaration");
}

void TopDecls(void)
{
	Scan();
	message("Top Declarations");
	while (TokenId != T_BEGIN) {
		switch(TokenId) {
		case T_VAR: Decl(); break;
		default:
			Abort("Unrecognised keyword - wanted VAR or BEGIN");
			break;
		}
		//Next();
		Scan();
	}
	message("  ");
	message("Top Declarations Done");
}

void Block(void)
{
	Scan();
	message("Block");
	while ( (TokenId != T_ENDIF) &&
	        (TokenId != T_ELSE) &&
	        (TokenId != T_END) && 
		(TokenId != T_ENDWHILE) )  {
		switch (TokenId) {
		case T_IF: DoIf(); break;
		case T_WHILE: DoWhile(); break;
		case T_READ: DoRead(); break;
		case T_WRITE: DoWrite(); break;
		default : Assignment(); break;
		}
		Scan();
	}
	message("Endblock");
}

void Init(void)
{
	int i;
	lineno = 1;
	labelno = 0;
	colno = 1;
	GetChar();
	Next();
	message("Init Done");
}

void Prog(void)
{
	message("Program");
	MatchString("PROGRAM");
	message("Starting Program");
	AsmHeader();
	message("TopDecls");
	TopDecls();

	message("main");
	MatchString("BEGIN");
	AsmProlog();
	Block();
	MatchString("END");
	AsmEpilog();
	message("endmain");

	//MatchString("."); // FIXME: do something proper with this
	message("Endprogram");
	//ShowSymTable();
}


/* -------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
	Init();
#ifdef SCAN_TEST
	while (Look != '.') {
		printf(".....\n");
		Next();
		printf("# Token: \"%s\" (%d)\n", Token, TokenId);
	}
	printf("# end main()\n")
#else
	Prog();
#endif
}

