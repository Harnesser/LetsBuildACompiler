//
// Scanning functions
//

char *Keywords[] = {
	"IF",
	"ELSE",
	"ENDIF",
	"END"
	};

#define NUMKEYWORDS sizeof(Keywords)/sizeof(Keywords[0])

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
	int iskw=0;
	int i=0;
	if (!IsAlpha(Look)) {
		Expected("Name");
	}
	while ( IsAlNum(Look) && i<MAXNAME-1 ) {
		Name[i++] = toupper(Look);
		GetChar();
	}
	Name[i] = '\0';
	iskw = Lookup(Name);
	if (iskw==0 ) {
		TokenId = T_IDENT;
	} else {
		TokenId = iskw;
	}
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
	TokenId = T_NUMBER;
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
	TokenId = T_OPER;
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
	int iskw;

	if ( IsAlpha(Look) ) {
		ident = GetName();
		iskw = Lookup(ident);
	} else if ( IsDigit(Look) ) {
		ident = GetNum();
	} else if ( IsOp(Look) ) {
		ident = GetOp();
	} else {
		clear_ident(Name);
		Name[0] = Look;
		ident = Name;
		GetChar();
		TokenId = T_OTHER;
	}
	SkipWhite();
	strncpy(Token, ident, MAXNAME);
	return Token;
}	

// If the input string matches an entry in the table, return
// the entry index+1. If not, return 0.
// brute force. maybe in future explore gperf
int Lookup(char *token) 
{
	int found;
	int i;

	//printf("Is %s in the list of keywords?\n", token);	
	i = NUMKEYWORDS;
	found = 0;
	while ( (i>0) && (found==0) ) {
		//printf(" \"%s\" vs \"%s\" -> ", Keywords[i-1], token);
		//printf(" %d\n", strncmp(token, Keywords[i-1], MAXNAME) );
		if ( strncmp(token, Keywords[i-1], MAXNAME) == 0 ) {
			found = 1;
		} else {
			i--;
		}
	}
	return i;
}	

