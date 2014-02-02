// Generate bits of ASM for function stuff.

void AsmProcedureBegin(char *name)
{
	printf(".section .text\n");
	printf(".type %s, @function\n", name);
	printf("%s:\n", name);

	// bookkeepping
	EmitLn("pushl %ebp");
	EmitLn("movl %esp, %ebp");
}

void AsmProcedureEnd(char *name)
{
	EmitLn("movl %ebp, %esp");
	EmitLn("popl %ebp");
	EmitLn("ret");
	printf("# End of %s\n", name);
}

void AsmProcedureCall(char *name)
{
	char code[MAXMSG];
	snprintf(code, MAXMSG, "call %s", name);
	EmitLn(code);
}
