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
	struct TDOP_Token prev;
	struct Scanner_Context scon;
};

void print_tdop_token(struct TDOP_Token *tok)
{
	printf("TDOP Token:\n");
	printf(" lbp: %d\n val: %d\n", tok->lbp, tok->val);
}

int expression(struct TDOP_Context *context, int lbp);
int match(struct TDOP_Context *context, TokenId id);

/* Functions called on tokens */
int tdop_num_nud(struct TDOP_Context *context)
{
	printf("Called tdop_num_nud, returning %d\n", context->prev.val);
	return context->prev.val;
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

int tdop_sub_nud(struct TDOP_Context *context)
{
	int val;
	printf("Called tdop_sub_nud...\n");
	val = -expression(context, 100); /* has huge rbp */
	printf(" returning %d\n", val);
	return val;
}

int tdop_sub_led(struct TDOP_Context *context, int left)
{
	int right;
	printf("Called tdop_sub_led\n");
	right = expression(context, 10);
	return left - right;
}

/* power operator is right-associative.
Want parser to treat subsequent exp ops as sub-expressions
of the first one. So call expression() with a rbp lower
than the lbp of the exponential.

When expression() gets to the next '^', it finds that still
rbp < token.lbp and won't return the result straight away,
but collects the value of the sub-expression first.*/
int tdop_pow_led(struct TDOP_Context *context, int left)
{
	int right;
	printf("Called tdop_pow_led\n");
	right = expression(context, 30-1);
	return pow(left, right);
}

int tdop_lbra_nud(struct TDOP_Context *context)
{
	int exprn;
	printf("Called tdop_lbra_nud\n");
	exprn = expression(context, 0);
	match(context, T_RBRA);
	return exprn;
}

/* grab next token from stream and make it into a parser token */
int next_token(struct TDOP_Context *context)
{
	int status;

	/* keep the current context around, we'll need this for
	calls to nud() to pick up the correct int value for the token
	after the scanner is advanced */
	context->prev = context->pcon;

	status = get_token(&context->scon);
	if (status != 0) {
		printf("Hit end of stream...\n");
		return 1;
	}

	/* Build a parsing token from the scanner token.
	The parser token will contain pointers to the appropriate
	nud() and led() functions, as well as the correct left and 
	right binding powers */
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
		break;
	case T_SUB:
		printf("Recognised subtraction\n");
		context->pcon.lbp = 10;
		context->pcon.val = 0;
		context->pcon.nud = &tdop_sub_nud;
		context->pcon.led = &tdop_sub_led;
		break;
	case T_POW:
		printf("Recognised exponential\n");
		context->pcon.lbp = 30;
		context->pcon.val = 0;
		context->pcon.nud = NULL;
		context->pcon.led = &tdop_pow_led;
		break;
	case T_LBRA:
		printf("Recognised open braces\n");
		context->pcon.lbp = 0;
		context->pcon.val = 0;
		context->pcon.nud = &tdop_lbra_nud;
		context->pcon.led = NULL;
		break;
	case T_RBRA:
		printf("Recognised close braces\n");
		context->pcon.lbp = 0;
		context->pcon.val = 0;
		context->pcon.nud = NULL;
		context->pcon.led = NULL;
		break;
	default:
		printf("oops - unrecognized token type\n");
		return 2;
		break;
	}

	return 0;
}

/* consume a token, but don't call anything on it */
int match(struct TDOP_Context *context, TokenId id)
{
	int status;
	printf("Matching %d\n", id);
	if (context->scon.t.id != id ) {
		printf("ERROR! expecting ')'\n");
		return 0;
	} else {
		printf("I see you...\n");
	}

	status = next_token(context);
	if (status != 0 ) {
		printf("Reached end of stream on a ')'\n");
	}
	printf("Leaving match()\n");
	return 1;
}

/* TDOP toplevel functions */
int expression(struct TDOP_Context *context, int rbp)
{
	int status;
	int left;
	int (*nud_fn)(struct TDOP_Context *context);
	int (*led_fn)(struct TDOP_Context *context, int left);

	printf("-------------------------------------------------------------------\n");
	printf("Entering expression function.\n");

	/* advance the scanner before we (might) recurse */
	status = next_token(context);
	if (status != 0 ) {
		printf("Ran out of expression (1).\n");
	}

	/* now, with the scanner advanced, we can call prev.nud() safe
	in the knowledge that any recursive expression() call has a new
	token in the pipe */
	nud_fn = context->prev.nud;
	assert(nud_fn != NULL);
	left = (*nud_fn)(context);
	printf(" left = %d\n", left);

	printf("RBP (%d) vs LBP (%d)\n", rbp, context->pcon.lbp );
	while (rbp < context->pcon.lbp ) {
		printf("----------------------------------------\n");

		/* advance the scanner */
		status = next_token(context);
		if (status != 0 ) {
			printf("Ran out of expression (2).\n");
			return 0;
		}

		/* with scanner advanced, call led() */
		led_fn = context->prev.led;
		assert(led_fn!= NULL);
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

