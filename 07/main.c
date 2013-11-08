// compiler cradle program
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const int TAB = '\t';
const int MAXMSG = 100;
const int MAXLBL = 8+1;
const int MAXNAME = 25+1;
const int MAXOPER = 5;

int lineno;
int colno;
int labelno;

typedef enum { T_OTHER=0, T_IF, T_ELSE, T_ENDIF, T_END,
               T_IDENT, T_NUMBER, T_OPER } e_token;

e_token TokenId;

char Look;  /* lookahead character */
char Token[26]; /* scanned token */
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
	Token[0] = '\0';
	TokenId = T_OTHER;
}

/* -------------------------------------------------------------------- */

// forward declarations
void Ident(void);
void Relation(void);
void Expression(void);

// stuff
#include "scanning.c"
#include "boolean.c"
#include "arithmetic.c"
#include "conditional.c"

/* -------------------------------------------------------------------- */

void Ident(void)
{
       char name[MAXNAME];
       char str[MAXMSG];

       GetName(name);
       if (Look=='(') {
               /* function call */
               Match('(');
               Match(')'); // empty arg list for now
               snprintf(str, MAXMSG, "bsr %s", name);
       } else {
               /* variable */
               snprintf(str, MAXMSG, "movl $%s, %%edx", name);
               EmitLn(str);
               EmitLn("movl (%edx), %eax");
       }       
}

void DoProgram(void)
{
	printf("# PROGRAM..\n");
	Block("");
	printf("# END OF BLOCK\n");
	MatchString("END");
	printf("#ENDPROGRAM\n");
}

/* -------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
	Init();
	//while (Look != EOF) {
	DoProgram();
	//}
}

