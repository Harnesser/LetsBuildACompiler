//
// IO Handlers
//

void DoRead()
{
	char name[MAXNAME];
	message("DoRead()");
	fflush(NULL);
	Match('(');
	GetName(name);
	ReadVar(name);
	while (Token[0]==',') {
		Match(',');
		GetName(name);
		ReadVar(name);
	}
	Match(')');
}


void DoWrite()
{
	Match('(');
	Expression();
	WriteVar();
	while (Token[0]==',') {
		Match(',');
		Expression();
		WriteVar();
	}
	Match(')');
}

