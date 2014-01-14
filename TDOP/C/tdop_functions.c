/* functions for TDOP parser */

struct TDOP_Context;

struct TDOP_Token {
	int lbp;
	int val;
	int (*nud)(struct TDOP_Context *context);
	int (*led)(struct TDOP_Context *context, int left);
};

struct TDOP_Context {
	struct TDOP_Token pt;
	struct Token t;
	struct Look l;
};

void print_tdop_token(struct TDOP_Token *pt)
{
	printf("TDOP Token:\n");
	printf(" lbp: %d\n val: %d\n", pt->lbp, pt->val);
}

int expression(struct TDOP_Context *context, int lbp);

int tdop_num_nud(struct TDOP_Context *context)
{
	printf("Called tdop_num_nud, returning %d\n", context->pt.val);
	return context->pt.val;
}

int tdop_add_led(struct TDOP_Context *context, int left)
{
	int right;
	printf("Called tdop_add_led\n");
	right = expression(context, 10);
	return left + right;
}

int next_token(struct TDOP_Context *context)
{
	int status;

	status = get_token(&context->t, &context->l);
	if (status != 0) {
		printf("Hit end of stream...\n");
		return 1;
	}

	switch(context->t.id) {
	case T_NUM:
		printf("Recognised number\n");
		context->pt.lbp = 0;
		context->pt.val = context->t.val;
		context->pt.nud = &tdop_num_nud;
		context->pt.led = NULL;
		break;
	case T_ADD:
		printf("Recognised addition\n");
		context->pt.lbp = 10;
		context->pt.val = 0;
		context->pt.nud = NULL;
		context->pt.led = &tdop_add_led;
		break;
	default:
		printf("oops - unrecognized token type\n");
		return 2;
		break;
	}

	return 0;
}

int expression(struct TDOP_Context *context, int rbp)
{
	int status;
	int left;
	
	printf("Entering expression function.\n");
	assert(context->pt.nud != NULL);
	left = (*context->pt.nud)(context);
	printf(" left = %d\n", left);

	status = next_token(context);
	if (status != 0 ) {
		printf("Ran out of expression.\n");
		return 0;
	}

	printf("RBP (%d) vs LBP (%d)\n", rbp, context->pt.lbp );
	while (rbp < context->pt.lbp ) {
		printf("----------------------------------------\n");
		assert(context->pt.led != NULL);
		left = (*context->pt.led)(context, left);

		status = next_token(context);
		if (status != 0 ) {
			printf("Ran out of expression (2).\n");
			return 0;
		}
	}
	printf("Leaving expression\n");
	return left;
}

void parse(void)
{
	struct TDOP_Context context;
	scanner_init(&context.t, &context.l);
	print_token(&context.t);
	assert( next_token(&context) == 0);
	printf("Expression %d\n", expression(&context, 0) );
/*	while ( next_token(&context) == 0) {
		print_token(&context.t);
		print_tdop_token(&context.pt);
	}
*/
}

