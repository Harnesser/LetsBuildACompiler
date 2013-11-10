
void Expression(void);

int IsAddop(char tok) {
	if ( (tok=='+') || (tok=='-') ) {
		return 1;
	}
	return 0;
}

void Assignment(void)
{
	char name;
	name = GetName();
	message("Assignment to %c", name);
	Match('=');
	BoolExpression();
	Store(name);
}


void Factor(void)
{
	message("Factor");
	if (Look == '(') {
		Match('(');
		BoolExpression();
		Match(')');
	} else if ( IsAlpha(Look) ) {
		LoadVar(GetName());
	} else {
		LoadConst(GetNum());
	}
}

void NegFactor(void)
{
	Match('-');
	if (IsDigit(Look)) {
		LoadConst(-GetNum());
	} else {
		Factor();
		Negate();
	}
}

void FirstFactor(void)
{
	if (Look=='+') {
		Match('+');
		Factor();
	} else if (Look=='-') {
		NegFactor();
	} else {
		Factor();
	}
}

void Multiply(void)
{
	Match('*');
	Factor();
	PopMul();
}

void Divide(void)
{
	Match('/');
	Factor();
	PopDiv();
}

void Term1(void)
{
	while ( (Look=='*') || (Look=='/') ) {
		Push();
		if (Look='*') {
			Multiply();
		} else if (Look='/') {
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
	Match('+');
	Term();
	PopAdd();
}

void Sub(void)
{
	Match('-');
	Term();
	PopSub();
}

void Expression(void)
{
	message("Expression");
	FirstTerm();
	while (IsAddop(Look)) {
		Push();
		if (Look=='+') {
			Add();
		} else if (Look=='-') {
			Sub();
		}
	}
}

