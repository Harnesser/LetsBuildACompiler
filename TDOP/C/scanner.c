/* scanner for TDOP example */

#define MAXTOKEN 25
typedef enum { T_NONE, T_IDENT, T_NUM, T_ADD, T_MUL, T_SUB } TokenId;

struct Token {
	char tok[MAXTOKEN];
	TokenId id;
	int val;
};

struct Scanner_Context {
	char look;
	struct Token t;
};

void skip_white(struct Scanner_Context *scon)
{
	while ( isspace(scon->look) ) {
		scon->look = getchar();
	}
}

void get_number(struct Scanner_Context *scon)
{
	int i;
	i = 0;
	scon->t.tok[i++] = scon->look;
	scon->t.val = (scon->look - '0');
	scon->look = getchar();
	while ( isdigit(scon->look) ) {
		scon->t.tok[i++] = scon->look; 
		scon->t.val = ( 10 * scon->t.val ) + (scon->look - '0');
		scon->look = getchar();
	}
	scon->t.tok[i] = '\0';
	scon->t.id = T_NUM;
}

void get_ident(struct Scanner_Context *scon)
{
	int i;
	i = 0;
	scon->t.tok[i++] = scon->look;
	scon->t.val = 0;
	scon->look = getchar();
	while ( isalpha(scon->look) ) {
		scon->t.tok[i++] = scon->look;
		scon->look = getchar();
	}
	scon->t.tok[i] = '\0';
	scon->t.id = T_IDENT;
}

void get_oper(struct Scanner_Context *scon)
{
	scon->t.tok[0] = scon->look;
	scon->t.tok[1] = '\0';
	scon->t.val = 0;
	printf("get_oper: c = \"%c\"\n", scon->look);
	switch (scon->look) {
	case '+' : scon->t.id = T_ADD; break;
	case '*' : scon->t.id = T_MUL; break;
	case '-' : scon->t.id = T_SUB; break;
	default  : scon->t.id = T_NONE; break;
	}
	scon->look = getchar();
}

int get_token(struct Scanner_Context *scon)
{
	if ( scon->look == EOF ) {
		return 1;
	}

	if ( isdigit(scon->look) ) {
		get_number(scon);
	} else if ( isalpha(scon->look) ) {
		get_ident(scon);
	} else {
		get_oper(scon);
	}
	skip_white(scon);
	return 0;
}

void scanner_init(struct Scanner_Context *scon)
{
	scon->t.val = 0;
	scon->t.tok[0] = '\0';
	scon->t.id = T_NONE;
	
	scon->look = getchar();
	skip_white(scon);
	//assert( get_token(t,l) == 0 );	
	
}

void print_token(struct Token *t)
{
        printf("STRUCT:\n");
	printf(" text = \"%s\"\n", t->tok);
        printf("  val = %d\n", t->val);
	printf("   id = %d\n", t->id);
}
