// compiler cradle program
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const int TAB = '\t';
const int MAXMSG = 100;
const int MAXLBL = 8+1;

int lineno;

char Look;  /* lookahead character */

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

int IsBoolean(char tok)
{
	char tokup;
	tokup = toupper(tok);
	if ( (tokup=='T') || (tokup=='F') ) {
		return 1;
	}
	return 0;
}

int IsOrOp(char tok)
{
	if ( (tok=='|') || (tok=='^') ) {
		return 1;
	}
	return 0;
}

int IsRelop(char tok)
{
	switch(tok){
	case '=': // fallthru
	case '>': // fallthru
	case '<': // fallthru
	case '#': // fallthru
		return 1;
		break; // yeah...
	}
	return 0;
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

int GetBoolean(void)
{
	int truth;
	char tokup;
	if (!IsBoolean(Look)) {
		Expected("Boolean Literal");
	}
	tokup = toupper(Look);
	truth = (tokup == 'T');
	GetChar();
	return truth;
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
	GetChar();
}

/* -------------------------------------------------------------------- */
void Relation(void);
void Expression(void);

void BoolFactor(void)
{
	if (IsBoolean(Look)) {
		if (GetBoolean()) {
			EmitLn("movl $-1,%eax");
		} else {
			EmitLn("movl $0,%eax");
		}
	} else {
		Relation();
	}
}

void NotFactor(void)
{
	if (Look=='!') {
		Match('!');
		BoolFactor();
		EmitLn("not %eax");
	} else {
		BoolFactor();
	}
}

void BoolTerm(void)
{
	NotFactor();
	while (Look=='&') {
		EmitLn("pushl %eax");
		Match('&');
		NotFactor();
		EmitLn("popl %edx");
		EmitLn("andl %edx, %eax");
	}
}

void BoolOr(void)
{
	Match('|');
	BoolTerm();
	EmitLn("popl %edx");
	EmitLn("orl %edx, %eax");
}

void BoolXor(void)
{
	Match('^');
	BoolTerm();
	EmitLn("popl %edx");
	EmitLn("xorl %edx, %eax");
}

void BoolExpression(void)
{
	BoolTerm();
	while (IsOrOp(Look)) {
		EmitLn("push %eax");
		switch(Look) {
		case '|': BoolOr(); break;
		case '^': BoolXor(); break;
		}
	}
}

/* NOTE: do logical opposite then subtract 1 to get true == all ones*/
void Equals(void){
	Match('=');
	Expression();
	EmitLn("popl %edx");
	EmitLn("cmp %eax, %edx");
	EmitLn("setne %al");
	EmitLn("movsx %al, %eax");
	EmitLn("subl $1, %eax");
}

void NotEquals(void){
	Match('#');
	Expression();
	EmitLn("popl %edx");
	EmitLn("cmp %eax, %edx");
	EmitLn("sete %al");
	EmitLn("movsx %al, %eax");
	EmitLn("subl $1, %eax");
}

void Less(void){
	Match('<');
	Expression();
	EmitLn("popl %edx");
	EmitLn("cmp %eax, %edx");
	EmitLn("setge %al");
	EmitLn("movsx %al, %eax");
	EmitLn("subl $1, %eax");
}

void Greater(void){
	Match('>');
	Expression();
	EmitLn("popl %edx");
	EmitLn("cmp %eax, %edx");
	EmitLn("setle %al");
	EmitLn("movsx %al, %eax");
	EmitLn("subl $1, %eax");
}


void Relation(void)
{
	Expression();
	if (!IsRelop(Look)) {
		return;
	}
	EmitLn("pushl %eax");
	switch(Look) {
		case '=' : Equals(); break;
		case '>' : Greater(); break;
		case '<' : Less(); break;
		case '#' : NotEquals(); break;
	}
	EmitLn("test %eax, %eax");
}

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

void Factor(void)
{
	char str[MAXMSG];
	if (Look=='(') {
		Match('(');
		Expression();
		Match(')');
	} else if (IsAlpha(Look)) {
		Ident();
	} else {
		snprintf(str, MAXMSG, "movl $%d,%%eax", GetNum() );
		EmitLn(str);
	}
}

void Multiply(void)
{
	Match('*');
	Factor();
	EmitLn("popl %ebx");
	EmitLn("imul %ebx");
}

void Divide(void)
{
	Match('/');
	Factor();
	EmitLn("movl %eax,%ebx");
	EmitLn("popl %eax");
	EmitLn("movl $0,%edx");
	EmitLn("divl %ebx");
}

void Term(void)
{
	Factor();
	while (Look=='*' || Look=='/') {
		EmitLn("pushl %eax");
		switch (Look) {
		case '*' : 
			Multiply();
			break;
		case '/' :
			Divide();
			break;
		default:
			Expected("Mulop");
			break;
		}
	}
}

void Add(void)
{
	Match('+');
	Term();
	EmitLn("popl %ebx");
	EmitLn("addl %ebx, %eax");
}

void Subtract(void)
{
	Match('-');
	Term();
	EmitLn("popl %ebx");
	EmitLn("subl %ebx, %eax");
	EmitLn("neg %eax");
}

int IsAddop(const char tok)
{
	if (tok=='-' || tok=='+' ) {
		return 1;
	}
	return 0;
}

void Expression(void)
{
	if (IsAddop(Look)) {
		EmitLn("clr %eax");
	} else {
		Term();
	}

	while (IsAddop(Look)) {
		EmitLn("pushl %eax");
		switch (Look) {
		case '+':
			Add();
			break;
		case '-':
			Subtract();
			break;
		default:
			Expected("Addop");
			break;
		}
	}
}

void Assignment(void)
{
	char name;
	char str[MAXMSG];
	name = GetName();
	Match('=');
	Expression();
	snprintf(str, MAXMSG, "movl $%c,%%edx", name);
	EmitLn(str);
	EmitLn("movl %eax,(%edx)");
}

/* -------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
	Init();
	while (Look != EOF) {
		while (Look=='\n') {
			Match('\n');
			lineno++;
		}
		BoolExpression();
		GetChar();
	}
}

