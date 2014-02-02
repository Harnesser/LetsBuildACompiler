//
// Arithmetic stuff
//

void Expression(void);

int IsAddop(char tok) {
	if ( (tok=='+') || (tok=='-') ) {
		return 1;
	}
	return 0;
}

void Assignment(void)
{
	char name[MAXNAME];
	strncpy(name, Token, MAXNAME);
	message("\n### Assignment to %s", name);
	Next(); // identifier
	MatchString("=");
	BoolExpression();
	Store(name);
}


void Factor(void)
{
	char name[MAXNAME];
	message("Factor");
	if (Token[0] == '(') {
		MatchString("(");
		BoolExpression();
		MatchString(")");
	} else if ( IsAlpha(Token[0]) ) {
		strncpy(name, Token, MAXNAME);
		LoadVar(name);
	} else {
		LoadConst(Value);
	}
	Next();
}

void NegFactor(void)
{
	MatchString("-");
	if (IsDigit(Token[0])) {
		LoadConst(-Value);
	} else {
		Factor();
		Negate();
	}
}

void FirstFactor(void)
{
	if (Token[0]=='+') {
		MatchString("+");
		Factor();
	} else if (Token[0]=='-') {
		NegFactor();
	} else {
		Factor();
	}
}

void Multiply(void)
{
	MatchString("*");
	Factor();
	PopMul();
}

void Divide(void)
{
	MatchString("/");
	Factor();
	PopDiv();
}

void Term1(void)
{
	while ( (Token[0]=='*') || (Token[0]=='/') ) {
		Push();
		if (Token[0]=='*') {
			Multiply();
		} else if (Token[0]=='/') {
			Divide();
		}
	}
}

void Term(void)
{
	Factor();
	Term1();
}

void FirstTerm(void)
{
	FirstFactor();
	Term1();
}

void Add(void)
{
	MatchString("+");
	Term();
	PopAdd();
}

void Sub(void)
{
	MatchString("-");
	Term();
	PopSub();
}

void Expression(void)
{
	message("Expression");
	FirstTerm();
	while (IsAddop(Token[0])) {
		Push();
		if (Token[0]=='+') {
			Add();
		} else if (Token[0]=='-') {
			Sub();
		}
	}
}

