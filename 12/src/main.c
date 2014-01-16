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
char TmpChar; /* used in comment spotting */

typedef enum { T_OTHER=0,
               T_IF, T_ELSE, T_ENDIF, T_END, T_BEGIN, T_VAR, T_WHILE, T_ENDWHILE,
               T_READ, T_WRITE,	
               T_PROGRAM_END,
               T_IDENT, T_NUMBER, T_OPER } e_token;

e_token TokenId;
char Token[26]; /* scanned token */
int Value; 

void GetCharX(void)
{
	Look = getchar();
	colno++;
}

/* we're going to tokenise comment chars as soon as we see them.

 The multi-line comment start char is the C one as at the beginning
of this comment. Every time we see a '/' in the stream, we'll
examine the next char to see if it is '*'. If is is not, the next
char is stored in TmpChar, and returned on the next call of this
function to avoid it going down the bitbucket. Otherwise we replace
the char with '@' which is added to the IsWhitespace test and so will
be skipped.

 To come out of a comment...
*/
void GetChar(void)
{
	if (TmpChar != ' ') {
		Look = TmpChar;
		TmpChar = ' ';
	} else {
		GetCharX();
		if (Look=='/') {
			TmpChar = getchar();
			colno++;
			if (TmpChar == '*') {
				Look = '@';
				TmpChar = ' ';
			}
		}
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

int IsWhite(const char tok) { return ( isspace(tok) || (tok=='@') ); }

void SkipComment(void);

void SkipWhite(void)
{
	//message("Chomping whitespace...");
	while (IsWhite(Look)) {
		if (Look == '\n') {
			lineno++;
			colno = 0;
		}
		if (Look == '@') {
			SkipComment();
		} else {
			GetChar();
		}
	}
	//message("Done chomping whitespace");
}

void Printable(char *pline, char tok)
{
	switch(tok) {
	case '\n': strncpy(pline, "NEWLINE", MAXNAME); break;
	case '\t': strncpy(pline, "TAB", MAXNAME); break;
	case '\0': strncpy(pline, "EOF", MAXNAME); break;
	default  : pline[0] = tok; pline[1] = '\0'; break;
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
		Semi();
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
		case T_IF:    DoIf(); break;
		case T_WHILE: DoWhile(); break;
		case T_READ:  DoRead(); Semi(); break;
		case T_WRITE: DoWrite(); Semi(); break;
		case T_IDENT: Assignment(); Semi(); break;
		default: break;
		}
		message("Here");
		Scan();
	}
	message("Endblock");
}

void Init(void)
{
	lineno = 1;
	labelno = 0;
	colno = 1;
	TmpChar = ' ';
	GetChar();
	Next();
	message("Init Done");
}

void Prog(void)
{
	message("Program");
	MatchString("PROGRAM");
	Semi();
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
	(void)argc; (void)argv; // so I can -Werror
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
	return 0;
}

