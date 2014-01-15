/* functions for TDOP parser */

struct TDOP_Context;

struct TDOP_Token {
	int lbp;
	int val;
	int (*nud)(struct TDOP_Context *context);
	int (*led)(struct TDOP_Context *context, int left);
};

struct TDOP_Context {
	struct TDOP_Token pcon;
	struct Scanner_Context scon;
};

void print_tdop_token(struct TDOP_Token *tok)
{
	printf("TDOP Token:\n");
	printf(" lbp: %d\n val: %d\n", tok->lbp, tok->val);
}

int expression(struct TDOP_Context *context, int lbp);

/* Functions called on tokens */
int tdop_num_nud(struct TDOP_Context *context)
{
	printf("Called tdop_num_nud, returning %d\n", context->pcon.val);
	return context->pcon.val;
}

int tdop_add_led(struct TDOP_Context *context, int left)
{
	int right;
	printf("Called tdop_add_led\n");
	right = expression(context, 10);
	return left + right;
}

int tdop_mul_led(struct TDOP_Context *context, int left)
{
	int right;
	printf("Called tdop_mul_led\n");
	right = expression(context, 20);
	return left * right;
}

/* grab next token from stream */
int next_token(struct TDOP_Context *context)
{
	int status;

	status = get_token(&context->scon);
	if (status != 0) {
		printf("Hit end of stream...\n");
		return 1;
	}

	switch(context->scon.t.id) {
	case T_NUM:
		printf("Recognised number\n");
		context->pcon.lbp = 0;
		context->pcon.val = context->scon.t.val;
		context->pcon.nud = &tdop_num_nud;
		context->pcon.led = NULL;
		break;
	case T_ADD:
		printf("Recognised addition\n");
		context->pcon.lbp = 10;
		context->pcon.val = 0;
		context->pcon.nud = NULL;
		context->pcon.led = &tdop_add_led;
		break;
	case T_MUL:
		printf("Recognised multiplication\n");
		context->pcon.lbp = 20;
		context->pcon.val = 0;
		context->pcon.nud = NULL;
		context->pcon.led = &tdop_mul_led;
	default:
		printf("oops - unrecognized token type\n");
		return 2;
		break;
	}

	return 0;
}

/* TDOP toplevel functions */
int expression(struct TDOP_Context *context, int rbp)
{
	int status;
	int left;
	int (*led_fn)(struct TDOP_Context *context, int left);
	
	printf("-------------------------------------------------------------------\n");
	printf("Entering expression function.\n");
	assert(context->pcon.nud != NULL);
	left = (*context->pcon.nud)(context);
	printf(" left = %d\n", left);

	status = next_token(context);
	if (status != 0 ) {
		printf("Ran out of expression (1).\n");
	}

	printf("RBP (%d) vs LBP (%d)\n", rbp, context->pcon.lbp );
	while (rbp < context->pcon.lbp ) {
		printf("----------------------------------------\n");
		/* store fn pointer - need to advance the scanner before
		 it's called */
		assert(context->pcon.led != NULL);
		led_fn = context->pcon.led;

		status = next_token(context);
		if (status != 0 ) {
			printf("Ran out of expression (2).\n");
			return 0;
		}
		left = (*led_fn)(context, left);
	}
	printf("Leaving expression with %d\n", left);
	return left;
}

void parse(void)
{
	struct TDOP_Context context;
	scanner_init(&context.scon);
	print_token(&context.scon.t);
	assert( next_token(&context) == 0);
	printf("RESULT: %d\n", expression(&context, 0) );
}

