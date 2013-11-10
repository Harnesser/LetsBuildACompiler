//
// Scanning functions
//

char *Keywords[] = {
	"IF",
	"ELSE",
	"ENDIF",
	"END",
	"BEGIN",
	"VAR"
	};

#define NUMKEYWORDS sizeof(Keywords)/sizeof(Keywords[0])

void NewLine(void)
{
	//message("NewLine?");
	while (Look=='\n') {
		GetChar();
		colno = 0;
		lineno++;
		SkipWhite();
	}
	//message("Eaten newlines.");
}


void Match(const char tok)
{
	char msg[MAXMSG];
	message("Looking to match \'%c\'", tok);
	NewLine();
	if (Look == tok) {
		GetChar();
		message("Matched");
	} else {
		snprintf(msg, MAXMSG, "\"%c\" Look=\"%c\"", tok, Look);
		Expected(msg);
	}
	SkipWhite();
}


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

// writes a name to the string at `name`	
void GetName(char *name)
{
	int i=0;
	if (!IsAlpha(Look)) {
		Expected("Name");
	}
	while ( IsAlNum(Look) && i<MAXNAME-1 ) {
		name[i++] = toupper(Look);
		GetChar();
	}
	name[i] = '\0';
	SkipWhite();
}

// writes a number string to `numstr`
int GetNum()
{
	int val = 0;
	if (!IsDigit(Look)) {
		Expected("Integer");
	}
	while ( IsDigit(Look) ) {
		val = (10*val) + (int)(Look - '0');
		GetChar();
	}
	TokenId = T_NUMBER;
	SkipWhite();
	return val;
}

// writes an operator to string `opstr`
void *GetOp(char *opstr)
{
	int i=0;
	if (!IsOp(Look)) {
		Expected("Operator");
	}
	while ( IsOp(Look) && i<MAXOPER-1 ) {
		opstr[i++] = Look;
		GetChar();
	}
	opstr[i] = '\0';
	TokenId = T_OPER;
	SkipWhite();
}

// match a string to the current read token
void MatchString(char *str)
{
	message("# Checking \"%s\" vs \"%s\"\n", str, Token);
	if ( strncmp(str, Token, MAXNAME) != 0 ) {
		Expected(str);
	}
}

void clear_ident(char *ident)
{
	int i;
	for (i=0; i<MAXNAME; i++) {
		ident[i] = '\0';
	}
}

// Scan sets Token and TokenID
void Scan(void) 
{
	NewLine();
	GetName(Token);
	TokenId = Lookup(Token);
	if (TokenId == T_OTHER) {
		TokenId = T_IDENT;
	}
	SkipWhite();
	NewLine();
	printf("# Scanned...\n");
	printf("#  Token: \"%s\"\n", Token);
	printf("#     Id: %d\n", TokenId);
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

