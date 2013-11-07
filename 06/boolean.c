//
// Boolean Stuff
//

// recognisers
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

/* -------------------------------------------------------------------- */

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

