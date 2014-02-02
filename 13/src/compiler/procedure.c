/* Stuff for Procedures */

/* Procedure Declarations */ 
void FormalParam(void)
{
	char name[MAXNAME];
	strncpy(name, Token, MAXNAME);
	Next(); // eat identifier
	message("Formal parameter: %s", name);

	Symtable_insert(LocalSymTable, name, LocalSymTable->num_symbols); // keep track of position
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
#ifdef ADSFASDF
	log_info("Global Symbol Table");
	Symtable_show(SymTable);
	log_info("Local Symbol Table");
	Symtable_show(LocalSymTable);
#endif
	message("Formal Parameter list done");
}

void DoProc(void)
{
	char name[MAXNAME];
	message("Procedure");
	MatchString("PROCEDURE");
	strncpy(name, Token, MAXNAME);
	Next();

	// set up a symbol table for this procedure
	LocalSymTable = Symtable_create();

	FormalList();
	Semi();
	AsmProcedureBegin(name);
        DoBeginBlock();
	AsmProcedureEnd(name);
	
	Symtable_destroy(LocalSymTable);
	LocalSymTable = NULL;
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
