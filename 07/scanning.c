//
// Scanning functions
//

int IsOp(char c)
{
	switch(c){
	// fallthru everywhere
	case '+':
	case '-':
	case '/':
	case '*':
	case '<':
	case '>':
	case ':':
	case '=':
		return 1;
		break;
	}
	return 0;
}

	
char *GetName(void)
{
	int i=0;
	if (!IsAlpha(Look)) {
		Expected("Name");
	}
	while ( IsAlNum(Look) && i<MAXNAME-1 ) {
		Name[i++] = toupper(Look);
		GetChar();
	}
	Name[i] = '\0';
	SkipWhite();
	return Name;
}

char *GetNum(void)
{
	int i=0;
	if (!IsDigit(Look)) {
		Expected("Integer");
	}
	while ( IsDigit(Look) && i<MAXNAME-1 ) {
		Num[i++] = Look;
		GetChar();
	}
	Num[i] = '\0';
	SkipWhite();
	return Num;
}

char *GetOp(void)
{
	int i=0;
	if (!IsOp(Look)) {
		Expected("Operator");
	}
	while ( IsOp(Look) && i<MAXOPER-1 ) {
		Oper[i++] = Look;
		GetChar();
	}
	Oper[i] = '\0';
	SkipWhite();
	return Oper;
}


void clear_ident(char *ident)
{
	int i;
	for (i=0; i<MAXNAME; i++) {
		ident[i] = '\0';
	}
}

char *Scan(void) 
{
	char *ident;
	if ( IsAlpha(Look) ) {
		ident = GetName();
	} else if ( IsDigit(Look) ) {
		ident = GetNum();
	} else if ( IsOp(Look) ) {
		ident = GetOp();
	} else {
		clear_ident(Name);
		Name[0] = Look;
		ident = Name;
		GetChar();
	}
	SkipWhite();
	strncpy(Token, ident, MAXNAME);
	return Token;
}	

