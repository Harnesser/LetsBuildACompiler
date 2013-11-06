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

void DoIf(char *exit_label);
void DoWhile(void);
void DoLoop(void);
void DoRepeat(void);
void DoFor(void);
void DoDo(void);
void DoBreak(char *exit_label);

void Block(char *exit_label)
{
	while ( (Look != 'e') && (Look !='l') && (Look !='u') ) {
		switch (Look) {
		case 'i':
			DoIf(exit_label);
			break;
		case 'w':
			DoWhile();
			break;
		case 'p':
			DoLoop();
			break;
		case 'r':
			DoRepeat();
			break;
		case 'f':
			DoFor();
			break;
		case 'd':
			DoDo();
			break;
		case 'b':
			DoBreak(exit_label);
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

void Expression(void)
{
	EmitLn("# <expression>");
}

void DoIf(char *exit_label)
{
        char code[MAXMSG];
	char l1[MAXLBL];
	char l2[MAXLBL];

	Match('i');
	NewLabel();
	strncpy(l1, label, MAXLBL);
	strncpy(l2, label, MAXLBL);
	Condition();

        snprintf(code, MAXMSG, "je .%s", l1);
	EmitLn(code);

	// here, tutorial 05 only matches 'e' once, after the if statement
	// this doesn't seem to work unless i add Look=='l' to the test in
	// Blocks().

	Block(exit_label);
	if (Look=='l') {
		Match('l');
		printf("#ELSE\n");
		NewLabel();
		strncpy(l2, label, MAXLBL);
		snprintf(code, MAXMSG, "jmp .%s", l2); 
		EmitLn(code);
		PostLabel(l1);
		Block(exit_label);
	}
	Match('e'); // ENDIF
	printf("#ENDIF\n");
	PostLabel(l2);
}

void DoWhile(void)
{
        char code[MAXMSG];
        char l1[MAXLBL];
        char l2[MAXLBL];

        Match('w');
        NewLabel();
        strncpy(l1, label, MAXLBL);
	NewLabel();
        strncpy(l2, label, MAXLBL);

        PostLabel(l1);
	Condition();

        snprintf(code, MAXMSG, "je .%s", l2);
        EmitLn(code);
	Block(l2);

	Match('e'); // ENDWHILE
	printf("#ENDWHILE\n");
	snprintf(code, MAXMSG, "jmp .%s", l1);
	EmitLn(code);

	PostLabel(l2);
}

void DoLoop(void)
{
        char code[MAXMSG];
        char l1[MAXLBL];
        char l2[MAXLBL];

        Match('p');
        NewLabel();
        strncpy(l1, label, MAXLBL);
	NewLabel();
	strncpy(l2, label, MAXLBL); // exit point

	PostLabel(l1);
	Block(l2);
	Match('e');
        snprintf(code, MAXMSG, "jmp .%s", l1);
        EmitLn(code);
	printf("#ENDLOOP\n");	
        PostLabel(l2);
}


void DoRepeat(void)
{
        char code[MAXMSG];
        char l1[MAXLBL];
        char l2[MAXLBL];

        Match('r');
        NewLabel();
        strncpy(l1, label, MAXLBL);
        NewLabel();
        strncpy(l2, label, MAXLBL);
        PostLabel(l1);
	Block(l2);
	Match('u');
	Condition();
        snprintf(code, MAXMSG, "je .%s", l1);
        EmitLn(code);

	PostLabel(l2); // exit point
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

void DoFor(void)
{
        char code[MAXMSG];
        char l1[MAXLBL];
        char l2[MAXLBL];

        Match('f');
        NewLabel();
        strncpy(l1, label, MAXLBL);
	NewLabel();
        strncpy(l2, label, MAXLBL);

	GetName();
	Match('=');
	Expression(); // expr1 = initial value
	// no TO? have to 'e' instead?
	Expression(); // expr2 = target value

	EmitLn("pushl <expr2>");
	EmitLn("pushl <expr1>");

        PostLabel(l1);
	EmitLn("popl, %ecx");
	EmitLn("popl, %eax");
	EmitLn("pushl, %eax");
	EmitLn("pushl, %ecx");

	// test
	EmitLn("cmp %eax, %ecx");
        snprintf(code, MAXMSG, "jg .%s", l2);
        EmitLn(code);
	
	Block(l2);

	// loopback
	EmitLn("popl %ecx");
	EmitLn("inc %eac");
	EmitLn("push %ecx");
	snprintf(code, MAXMSG, "jmp .%s", l1);
        EmitLn(code);
	PostLabel(l2);

	Match('e'); // ENDWHILE
	printf("#ENDFOR\n");

}

void DoDo(void)
{
        char code[MAXMSG];
        char l1[MAXLBL];
        char l2[MAXLBL];

        Match('d');
        NewLabel();
        strncpy(l1, label, MAXLBL);
        NewLabel();
        strncpy(l2, label, MAXLBL); // exit point

	Expression(); // expr1 = repeat count
	EmitLn("pushl <expr1>");
	EmitLn("popl %eac");
	EmitLn("dec %eac");
	EmitLn("pushl %eac");

        PostLabel(l1);
	Block(l2);

	EmitLn("popl, %ecx");
	EmitLn("dec %ecx");
	EmitLn("pushl, %ecx");

	// test
        snprintf(code, MAXMSG, "jnz .%s", l1);
        EmitLn(code);
	
	Match('e'); // ENDWHILE
	printf("#ENDDO\n");
	PostLabel(l2);
}

void DoBreak(char *exit_label)
{
	char code[MAXMSG];
	Match('b');
	if ( strncmp(exit_label, "", MAXLBL) != 0 ) {
		snprintf(code, MAXMSG, "jmp .%s", exit_label);
		EmitLn(code);
	} else {
		Abort("No loop to break from");
	}
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

