/* Stuff for Procedures */

/* Procedure Declarations */ 
void FormalParam(void)
{
	char name[MAXNAME];
	strncpy(name, Token, MAXNAME);
	Next(); // eat identifier
	message("Formal parameter: %s", name);
}

void FormalList(void)
{
	message("Formal Parameter List");
	MatchString("(");
	if (Token[0] != ')') {
		FormalParam();
		while (Token[0] == ',') {
			MatchString(",");
			FormalParam();
		}
	}
	MatchString(")");
	message("Formal Parameter list done");
}

void DoProc(void)
{
	char name[MAXNAME];
	message("Procedure");
	MatchString("PROCEDURE");
	strncpy(name, Token, MAXNAME);
	Next();
	FormalList();
	Semi();
	AsmProcedureBegin(name);
        DoBeginBlock();
	AsmProcedureEnd(name);
        message("Endprocedure");
}

/* Procedure Calls */
void Param(void)
{
	char name[MAXNAME];
	strncpy(name, Token, MAXNAME);
	Next(); // eat identifier
	message("Formal parameter: %s", name);
}

void ParamList(void)
{
	message("Parameter List");
	MatchString("(");
	if (Token[0] != ')') {
		Param();
		while (Token[0] == ',') {
			MatchString(",");
			Param();
		}
	}
	MatchString(")");
	message("Parameter list done");
}

void DoProcCall(void)
{
	char name[MAXNAME];
	strncpy(name, Token, MAXNAME);
	message("Calling %s", name);
	Next(); // eat identifier
	ParamList();
	Semi();
	AsmProcedureCall(name);
}
