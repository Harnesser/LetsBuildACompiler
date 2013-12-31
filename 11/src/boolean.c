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
	Match('=');
	Expression();
	PopCompare();
	SetEqual();
}

void NotEquals(void)
{
	Match('>');
	Expression();
	PopCompare();
	SetNEqual();
}

void LessOrEqual(void)
{
	Match('=');
	Expression();
	PopCompare();
	SetLessOrEqual();
}

void Less(void)
{
	Match('<');
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
	Match('>');
	if ( Token[0]=='=') {
		Match('=');
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
		Match('!');
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
		Match('&');
		NotFactor();
		PopAnd();
	}
}

void BoolOr(void)
{
	Match('|');
	BoolTerm();
	PopOr();
}

void BoolXor(void)
{
	Match('^');
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


