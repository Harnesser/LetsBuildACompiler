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
	level++;
	strncpy(name, Token, MAXNAME);
	message("\n### Assignment to %s", name);
	Next(); // identifier
	MatchString("=");
	BoolExpression();
	Store(name);
	level--;
}


void Factor(void)
{
	char name[MAXNAME];
	level++;
	message("Factor");
	if ( (TokenId == T_OTHER ) && (Token[0] == '(') ) {
		MatchString("(");
		BoolExpression();
		MatchString(")");
	} else if ( IsAlpha(Token[0]) ) {
		strncpy(name, Token, MAXNAME);
		/* decide whether this is a procedure call or a variable */
		/* functions are registered only in the main symbol table as I'm
		   not allowing nested procedure declarations.*/
		if (IsProcedure(Token)) {
			DoProcCall();
		} else { /* variable */
			LoadVar(name);
			Next();
		}
	} else {
		LoadConst(Value);
		Next();
	}
	message("End factor");
	level--;
}

void NegFactor(void)
{
	level++;
	message("NegFactor");
	MatchString("-");
	if (IsDigit(Token[0])) {
		LoadConst(-Value);
	} else {
		Factor();
		Negate();
	}
	level--;
}

void FirstFactor(void)
{
	level++;
	message("First Factor");
	if (Token[0]=='+') {
		MatchString("+");
		Factor();
	} else if (Token[0]=='-') {
		NegFactor();
	} else {
		Factor();
	}
	level--;
}

void Multiply(void)
{
	level++;
	message("multiply");
	MatchString("*");
	Factor();
	PopMul();
	level--;
}

void Divide(void)
{
	level++;
	MatchString("/");
	Factor();
	PopDiv();
	level--;
}

void Term1(void)
{
	level++;
	message("Term1");
	while ( (Token[0]=='*') || (Token[0]=='/') ) {
		Push();
		if (Token[0]=='*') {
			Multiply();
		} else if (Token[0]=='/') {
			Divide();
		}
	}
	level--;
}

void Term(void)
{
	level++;
	message("Term");
	Factor();
	Term1();
	level--;
}

void FirstTerm(void)
{
	level++;
	message("First Term");
	FirstFactor();
	Term1();
	level--;
}

void Add(void)
{
	level++;
	message("Add");
	MatchString("+");
	Term();
	PopAdd();
	level--;
}

void Sub(void)
{
	MatchString("-");
	Term();
	PopSub();
}

void Expression(void)
{
	level++;
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
	message("End of expression");
	level--;
}

