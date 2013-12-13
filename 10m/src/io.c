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
	while (Look==',') {
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
	while (Look==',') {
		Match(',');
		Expression();
		WriteVar();
	}
	Match(')');
}

