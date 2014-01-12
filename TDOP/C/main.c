/* A C version of a Top-Down Operator Precedence (TDOP) scanner
 for expressions
*/

#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#include "scanner.c"
#include "tdop_functions.c"

void print_token(struct Token *t)
{
        printf("STRUCT:\n");
	printf(" text = \"%s\"\n", t->tok);
        printf("  val = %d\n", t->val);
	printf("   id = %d\n", t->id);
}

int main(int argc, char *argv[]) 
{
	(void)argc; (void)argv;

	struct Token t;
	struct Look l;
	scanner_init(&t, &l);
	print_token(&t);
	while ( get_token(&t, &l) == 0) {
		print_token(&t);
	}
	return 0;
}

