/* Stuff for Procedures */

/* Procedure Declarations */ 
void FormalParam(void)
{
	char name[MAXNAME];
	strncpy(name, Token, MAXNAME);
	Next(); // eat identifier
	message("Formal parameter: %s", name);

	Symtable_insert(LocalSymTable, name, SYM_FUNC + LocalSymTable->num_symbols); // keep track of position
}

int FormalList(void)
{
	int num_args = 0;
	message("Formal Parameter List");
	MatchString("(");
	if (Token[0] != ')') {
		FormalParam();
		num_args++;
		while (Token[0] == ',') {
			MatchString(",");
			FormalParam();
			num_args++;
		}
	}
	MatchString(")");
	message("Formal Parameter list done");
	return num_args; 
}

/* poop out the answer in %eax */
void DoProc(void)
{
	int num_args=0;
	char name[MAXNAME];
	message("Procedure");
	MatchString("PROCEDURE");
	strncpy(name, Token, MAXNAME);
	Next();
	
	LocalSymTable = Symtable_create();
	num_args = FormalList();
	Symtable_insert(SymTable, name, SYM_FUNC+num_args);
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
	message("Function call parameter");
	Expression();
	Push();
}

void ParamList(int expected_num_args)
{
	int num_args=0;
	message("Parameter List");
	MatchString("(");
	if (Token[0] != ')') {
		Param();
		num_args++;
		while (Token[0] == ',') {
			MatchString(",");
			Param();
			num_args++;
		}
	}
	MatchString(")");
	if (num_args != expected_num_args) {
		Abort("Parameter Count Mismatch");
	}
	message("Parameter list done");
}

void DoProcCall(void)
{
	int expected_num_args=0;
	char name[MAXNAME];
	strncpy(name, Token, MAXNAME);
	message("Calling %s", name);
	Next(); // eat identifier
	expected_num_args = Symtable_get(SymTable, name) - SYM_FUNC;
	ParamList(expected_num_args);
	Semi();
	AsmProcedureCall(name);
	CleanupStack(expected_num_args);
}
