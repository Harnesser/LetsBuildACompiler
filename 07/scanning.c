//
// Scanning functions
//

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

