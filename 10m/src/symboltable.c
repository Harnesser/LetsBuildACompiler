//
// Symbol table hacks
//

// The symbol table is an array of characters which is looked up 
// linearly. No hashmappimng or whatever.

int NumSymbols = 0;
char SymTable[100][26];

int InTable(char *ident)
{
	int found;
	int i;

	//printf("Is %s in the list of keywords?\n", token);	
	i = NumSymbols;
	found = 0;
	while ( (i>0) && (found==0) ) {
		//printf(" \"%s\" vs \"%s\" -> ", Keywords[i-1], token);
		//printf(" %d\n", strncmp(token, Keywords[i-1], MAXNAME) );
		if ( strncmp(ident, SymTable[i-1], MAXNAME) == 0 ) {
			found = 1;
		} else {
			i--;
		}
	}
	return i;
}

void AddEntry(char *ident)
{
	char msg[MAXMSG];
	if ( InTable(ident) ) {
		snprintf(msg, MAXMSG,
		    "Dupe variable declaration : \'%s\'", ident);
		Abort(msg);
	}

	if ( NumSymbols == MAXENTRY ) {
		Abort("Symbol table full");
	}

	strncpy( SymTable[NumSymbols], ident, MAXNAME);
	NumSymbols++;
}

void ShowSymTable(void)
{
	int i;
	for( i=NumSymbols; i>0; i-- ) {
		printf("ST[%d] = \t %s\n", i, SymTable[i-1]);
	}
}
