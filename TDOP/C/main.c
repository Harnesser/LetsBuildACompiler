/* A C version of a Top-Down Operator Precedence (TDOP) scanner
 for expressions
*/

#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>

#include "scanner.c"
#include "tdop_functions.c"

int main(int argc, char *argv[]) 
{
	(void)argc; (void)argv;

	parse();
	return 0;
}

