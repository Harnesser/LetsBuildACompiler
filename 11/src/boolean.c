//
// Boolean Stuff
//

// basic recognisers
int IsOrop(char c)
{
	if ( (Token[0]=='|') || (Token[0]=='^') ) {
		return 1;
	}
	return 0;
}

int IsRelop(char c)
{

	switch (c) {
	case '=':
	case '#':
	case '<':
	case '>':
		return 1;
		break;
	}
	return 0;

}

// operations
void Equals(void)
{
	MatchString("=");
	Expression();
	PopCompare();
	SetEqual();
}

void NotEquals(void)
{
	MatchString(">");
	Expression();
	PopCompare();
	SetNEqual();
}

void LessOrEqual(void)
{
	MatchString("=");
	Expression();
	PopCompare();
	SetLessOrEqual();
}

void Less(void)
{
	MatchString("<");
	switch (Token[0]) {
	case '=': LessOrEqual(); break;
	case '>': NotEquals(); break;
	default :	
		Expression();
		PopCompare();
		SetLess();
		break;
	}
}

void Greater(void)
{
	MatchString(">");
	if ( Token[0]=='=') {
		MatchString("=");
		Expression();
		PopCompare();
		SetGreaterOrEqual();
	} else {
		Expression();
		PopCompare();
		SetGreater();
	}
}

void Relation(void)
{
	message("Relation...");
	Expression();
	if (IsRelop(Token[0]) ) {
		message("  (Relation \'%c\')", Token[0]);
		Push();
		switch (Token[0]) {
		case '=': Equals(); break;
		case '#': NotEquals(); break;
		case '<': Less(); break;
		case '>': Greater(); break;
		}
	}
}

void NotFactor(void)
{
	message("NotFactor");
	if (Token[0]=='!') {
		MatchString("!");
		Relation();
		NotIt();
	} else {
		Relation();
	}
}

void BoolTerm(void)
{
	NotFactor();
	while (Token[0]=='&') {
		Push();
		MatchString("&");
		NotFactor();
		PopAnd();
	}
}

void BoolOr(void)
{
	MatchString("|");
	BoolTerm();
	PopOr();
}

void BoolXor(void)
{
	MatchString("^");
	BoolTerm();
	PopXor();
}

void BoolExpression(void)
{
	message("BoolExpression");
	BoolTerm();
	while (IsOrop(Token[0])) {
		switch (Token[0]) {
		case '|': BoolOr(); break;
		case '^': BoolXor(); break;
		}
	}
}


