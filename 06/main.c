// compiler cradle program
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const int TAB = '\t';
const int MAXMSG = 100;
const int MAXLBL = 8+1;

int lineno;
int colno;
int labelno;

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
void Ident(void);
void Relation(void);
void Expression(void);

// stuff
#include "boolean.c"
#include "arithmetic.c"
#include "conditional.c"

/* -------------------------------------------------------------------- */

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

void DoProgram(void)
{
	Block("");
	if (Look != 'e') {
		Expected("End");
	}
	Match('e');
	printf("#ENDPROGRAM\n");
}

/* -------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
	Init();
	while (Look != EOF) {
		DoProgram();
	}
}

