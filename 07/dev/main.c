// compiler cradle program
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const int TAB = '\t';
const int MAXMSG = 100;
const int MAXLBL = 8+1;
const int MAXNAME = 25;
const int MAXOPER = 5;

int lineno;
int colno;
int labelno;
typedef enum { T_IF, T_ELSE, T_ENDIF, T_END, T_IDENT, T_NUMBER, T_OPER, T_OTHER } e_token;
e_token TokenId;

char Name[25];  /* identifier name */
char Num[25];   /* number string */
char Token[25]; /* scanned token */
char Oper[5]; /* an operator token */

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
	int iskw;
	Init();
	while (Look != EOF || Look != '.') {
		Scan();
		iskw = Lookup(Token);
		switch(TokenId) {
		case(T_IDENT)  : printf("IDENT: "); break;
		case(T_OPER)   : printf("OPERATOR: "); break;
		case(T_NUMBER) : printf("NUMBER: "); break;
		case(T_IF)     :
		case(T_ELSE)   :
		case(T_ENDIF)  :
		case(T_END)    : printf("KEYWORD: "); break;
		default  : printf("< ?????? >: "); break;
		}
		printf("%s\n", Token);
		if (Token[0]=='\n') {
			Fin();
		}
	}
}

