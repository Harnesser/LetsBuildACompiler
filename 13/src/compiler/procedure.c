/* Functions to deal with procedure calls */

void DoProcCall(void)
{
	char name[MAXNAME];
	strncpy(name, Token, MAXNAME);
	message("Calling %s", name);
	Next(); // eat identifier
	MatchString("(");
	MatchString(")");
	AsmProcedureCall(name);
}
