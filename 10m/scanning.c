//
// Scanning Routines
//


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

char GetName(void)
{
	char name;
	NewLine();
	if (!IsAlpha(Look)) {
		Expected("Name");
	}
	name = toupper(Look);
	printf("# >>>> Name '%c' (%d)\n", name, name)	;
	GetChar();
	message("Name: %c", name);
	SkipWhite();
	return name;
}


int GetNum(void)
{
	char num;
	char x;
	NewLine();
	if (!IsDigit(Look)) {
		Expected("Integer");
	}
	num = 0;
	while (IsDigit(Look)) {
		x = (int)(Look - '0');
		num = num*10 + x;
		GetChar();
	}
	SkipWhite();
	return num;
}
