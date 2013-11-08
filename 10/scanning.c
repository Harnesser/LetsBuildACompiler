//
// Scanning Routines
//

char GetName(void)
{
	char name;
	if (!IsAlpha(Look)) {
		Expected("Name");
	}
	name = toupper(Look);
	printf("# >>>> Name '%c' (%d)\n", name, name)	;
	GetChar();
	message("Name: %c", name);
	return name;
}


int GetNum(void)
{
	char num;
	char x;
	if (!IsDigit(Look)) {
		Expected("Integer");
	}
	num = 0;
	while (IsDigit(Look)) {
		x = (int)(Look - '0');
		num = num*10 + x;
		GetChar();
	}
	return num;
}

