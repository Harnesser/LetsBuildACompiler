//
// Arithmetic Stuff
//

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

/* -------------------------------------------------------------------- */

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
	BoolExpression();
	snprintf(str, MAXMSG, "movl $%c,%%edx", name);
	EmitLn(str);
	EmitLn("movl %eax,(%edx)\t\t# assignment");
}


