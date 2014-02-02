//
// Conditional Stuff
//

void NewLabel(void){
	snprintf(label, MAXLBL, "L%d", labelno);
	labelno++;
}

void PostLabel(char *label)
{
        printf(".%s:\n", label);
}


void DoIf(void)
{
	char l1[MAXLBL];
	char l2[MAXLBL];
	message("IF...");

	Next();
	BoolExpression();	
	NewLabel();
	strncpy(l1, label, MAXLBL);
	strncpy(l2, label, MAXLBL);

	// true 
	BranchFalse(l1);
	message("IF (true)");
	Block();
	
	message("IF - checking for ELSE");
	if (TokenId == T_ELSE) {
		// false
		MatchString("ELSE");
		NewLabel();
		strncpy(l2, label, MAXLBL);
		Branch(l2);
		message("IF (false)");
		PostLabel(l1);
		Block();
	}
	
	message("IF exit");
	PostLabel(l2);
	MatchString("END");
}

void DoWhile(void)
{
	char l1[MAXLBL];
	char l2[MAXLBL];
	message("WHILE...");

	MatchString("WHILE");
	NewLabel();
	strncpy(l1, label, MAXLBL);
	NewLabel();
	strncpy(l2, label, MAXLBL);

	PostLabel(l1);
	BoolExpression();
	BranchFalse(l2);
	Block();
	MatchString("END");
	Branch(l1);
	PostLabel(l2);
	message("WHILE exit");
}
