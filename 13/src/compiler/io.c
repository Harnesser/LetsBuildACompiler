//
// IO Handlers
//

void DoRead()
{
	char name[MAXNAME];
	message("DoRead()");
	fflush(NULL);
	MatchString("READ");
	MatchString("(");
	strncpy(name, Token, MAXNAME);
	ReadVar(name);
	Next(); // chomp name
	while (Token[0]==',') {
		MatchString(",");
		strncpy(name, Token, MAXNAME);
		ReadVar(name);
		Next(); // chomp name
	}
	MatchString(")");
}


void DoWrite()
{
	MatchString("WRITE");
	MatchString("(");
	Expression();
	WriteVar();
	while (Token[0]==',') {
		MatchString(",");
		Expression();
		WriteVar();
	}
	MatchString(")");
}

