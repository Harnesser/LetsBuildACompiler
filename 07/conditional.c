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

void DoIf(char *exit_label);
void DoWhile(void);
void DoLoop(void);
void DoRepeat(void);
void DoFor(void);
void DoDo(void);
void DoBreak(char *exit_label);

void Block(char *exit_label)
{
	while ( (Look != 'e') && (Look !='l') && (Look !='u') ) {
		Fin();
		printf("# BLOCK start\n");
		switch (Look) {
		case 'i':
			DoIf(exit_label);
			break;
		case 'w':
			DoWhile();
			break;
		case 'p':
			DoLoop();
			break;
		case 'r':
			DoRepeat();
			break;
		case 'f':
			DoFor();
			break;
		case 'd':
			DoDo();
			break;
		case 'b':
			DoBreak(exit_label);
			break;
		default:
			Assignment();
			break;
		}
		Fin();
		printf("# BLOCK end\n");
	}
}


void DoIf(char *exit_label)
{
        char code[MAXMSG];
	char l1[MAXLBL];
	char l2[MAXLBL];

	Match('i');
	printf("# IF\n");
	NewLabel();
	strncpy(l1, label, MAXLBL);
	strncpy(l2, label, MAXLBL);
	BoolExpression();

        snprintf(code, MAXMSG, "je .%s", l1);
	EmitLn(code);
	printf("# TRUE\n");
	// here, tutorial 05 only matches 'e' once, after the if statement
	// this doesn't seem to work unless i add Look=='l' to the test in
	// Blocks().

	Block(exit_label);
	if (Look=='l') {
		Match('l');
		printf("#ELSE\n");
		NewLabel();
		strncpy(l2, label, MAXLBL);
		snprintf(code, MAXMSG, "jmp .%s", l2); 
		EmitLn(code);
		PostLabel(l1);
		Block(exit_label);
	}
	Match('e'); // ENDIF
	printf("#ENDIF\n");
	PostLabel(l2);
}

void DoWhile(void)
{
        char code[MAXMSG];
        char l1[MAXLBL];
        char l2[MAXLBL];

        Match('w');
        NewLabel();
        strncpy(l1, label, MAXLBL);
	NewLabel();
        strncpy(l2, label, MAXLBL);

        PostLabel(l1);
	BoolExpression();

        snprintf(code, MAXMSG, "je .%s", l2);
        EmitLn(code);
	Block(l2);

	Match('e'); // ENDWHILE
	printf("#ENDWHILE\n");
	snprintf(code, MAXMSG, "jmp .%s", l1);
	EmitLn(code);

	PostLabel(l2);
}

void DoLoop(void)
{
        char code[MAXMSG];
        char l1[MAXLBL];
        char l2[MAXLBL];

        Match('p');
        NewLabel();
        strncpy(l1, label, MAXLBL);
	NewLabel();
	strncpy(l2, label, MAXLBL); // exit point

	PostLabel(l1);
	Block(l2);
	Match('e');
        snprintf(code, MAXMSG, "jmp .%s", l1);
        EmitLn(code);
	printf("#ENDLOOP\n");	
        PostLabel(l2);
}


void DoRepeat(void)
{
        char code[MAXMSG];
        char l1[MAXLBL];
        char l2[MAXLBL];

        Match('r');
	printf("# REPEAT\n");
        NewLabel();
        strncpy(l1, label, MAXLBL);
        NewLabel();
        strncpy(l2, label, MAXLBL);
        PostLabel(l1);
	Block(l2);
	Match('u');
	printf("# UNTIL\n");
	BoolExpression();
        snprintf(code, MAXMSG, "je .%s\t\t#repeat loop", l1);
        EmitLn(code);

	PostLabel(l2); // exit point
}

void DoFor(void)
{
	char name;
        char code[MAXMSG];
        char l1[MAXLBL];
        char l2[MAXLBL];

        Match('f');
	printf("# FOR\n");
        NewLabel();
        strncpy(l1, label, MAXLBL);
	NewLabel();
        strncpy(l2, label, MAXLBL);

	// could call Assignment here, but I need to keep the loop counter handle
	name = GetName();
	Match('=');
	Expression(); // expr1 = initial value
	snprintf(code, MAXMSG, "movl $%c,%%edx", name);
	EmitLn(code);
	EmitLn("movl %eax,(%edx)\t\t# assignment");

	Expression(); // expr2 = target value
	EmitLn("pushl %eax    \t\t # put target count on stack");

        // Loop: 
	PostLabel(l1);
	EmitLn("popl %eax  \t\t# recover tgt");
	EmitLn("pushl %eax");
        snprintf(code, MAXMSG, "movl $%c,%%edx", name);
        EmitLn(code);
	EmitLn("movl (%edx), %ecx  \t\t # Grab loop counter");

	// test
	EmitLn("cmpl %eax, %ecx\t\t# for test");
        snprintf(code, MAXMSG, "jg .%s", l2);
        EmitLn(code);
	
	Block(l2);
	//EmitLn("popl %ecx  \t\t# don't need return val");

	// loopback
        snprintf(code, MAXMSG, "movl $%c,%%edx", name); 
	EmitLn(code);
	EmitLn("movl (%edx), %ecx    \t\t # grab loop counter");
	EmitLn("inc %ecx  \t\t # i++");
	EmitLn("movl %ecx, (%edx)");
	snprintf(code, MAXMSG, "jmp .%s", l1);
        EmitLn(code);
	PostLabel(l2);

	Match('e'); // ENDWHILE 
	EmitLn("popl %eax");

	printf("#ENDFOR\n");

}

void DoDo(void)
{
        char code[MAXMSG];
        char l1[MAXLBL];
        char l2[MAXLBL];

        Match('d');
        NewLabel();
        strncpy(l1, label, MAXLBL);
        NewLabel();
        strncpy(l2, label, MAXLBL); // exit point
	printf("# DO\n");

	Expression(); // expr1 = repeat count
	EmitLn("pushl %eax\t\t# repeat count");

        PostLabel(l1);
	Block(l2);

	EmitLn("popl %ecx");
	EmitLn("dec %ecx");
	EmitLn("pushl %ecx");

	// test
        snprintf(code, MAXMSG, "jnz .%s", l1);
        EmitLn(code);
	
	Match('e'); // ENDWHILE
	printf("#ENDDO\n");
	PostLabel(l2);
}

void DoBreak(char *exit_label)
{
	char code[MAXMSG];
	Match('b');
	if ( strncmp(exit_label, "", MAXLBL) != 0 ) {
		snprintf(code, MAXMSG, "jmp .%s", exit_label);
		EmitLn(code);
	} else {
		Abort("No loop to break from");
	}
}

