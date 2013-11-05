// compiler cradle program
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const int TAB = '\t';
const int MAXMSG = 100;
const int MAXLBL = 8+1;

int lineno;
int labelno;

char Look;  /* lookahead character */
char label[9]; /* label for machine code  conditionals */

void GetChar(void)
{
	Look = getchar();
}

void Error(const char *msg)
{
	printf("\n");
	printf("Error: \"%s\" at line %d\n", msg, lineno);
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

int GetNum(void)
{
	char num;
	if (!IsDigit(Look)) {
		Expected("Integer");
	}
	num = Look;
	GetChar();
	return (int)(num - '0');
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
	GetChar();
}

/* -------------------------------------------------------------------- */
void Other(void)
{
	char tok;
	char msg[MAXMSG];
	tok = GetName();
	snprintf(msg, MAXMSG, "# \"%c\"", tok);
	EmitLn(msg);
}

void NewLabel(void){
	snprintf(label, MAXLBL, "L%d", labelno);
	labelno++;
}

void PostLabel(char *label)
{
        printf(".%s:\n", label);
}

void DoIf(void);

void Block(void)
{
	while ( Look != 'e' ) {
		switch (Look) {
		case 'i':
			DoIf();
			break;
		case 'o':
			Other();
			break;
		default:
			Other();
			break;
		}
	}
}

void Condition(void)
{
	EmitLn("# <condition>");
}

void DoIf(void)
{
        char code[MAXMSG];
	char lbl[MAXLBL];

	Match('i');
	NewLabel();
	strncpy(lbl, label, MAXLBL);

	Condition();

        snprintf(code, MAXMSG, "jz .%s", lbl);
	EmitLn(code);

	Block();

	Match('e');
	PostLabel(lbl);
}

void DoProgram(void)
{
	Block();
	if (Look != 'e') {
		Expected("End");
	}
	Match('e');
	EmitLn("#END");
}


/* -------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
	Init();
	while (Look != EOF) {
		DoProgram();
		while (Look=='\n') {
			Match('\n');
			lineno++;
		}
	}
}

