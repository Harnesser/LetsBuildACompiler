//
// Boolean Stuff
//

// basic recognisers
int IsOrop(char c)
{
	if ( (Look=='|') || (Look=='^') ) {
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
	switch (Look) {
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
	if ( Look=='=') {
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
	if (IsRelop(Look) ) {
		message("  (Relation \'%c\')", Look);
		Push();
		switch (Look) {
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
	if (Look=='!') {
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
	while (Look=='&') {
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
	while (IsOrop(Look)) {
		switch (Look) {
		case '|': BoolOr(); break;
		case '^': BoolXor(); break;
		}
	}
}


