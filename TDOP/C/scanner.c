/* scanner for TDOP example */

#define MAXTOKEN 25
typedef enum { T_NONE, T_IDENT, T_NUM, T_ADD, T_MUL } TokenId;

struct Token {
	char tok[MAXTOKEN];
	TokenId id;
	int val;
};

struct Look {
	char c;
};

void skip_white(struct Look *look)
{
	while ( isspace(look->c) ) {
		look->c = getchar();
	}
}

void get_number(struct Token *t, struct Look *look)
{
	int i;
	i = 0;
	t->tok[i++] = look->c;
	t->val = (look->c - '0');
	look->c = getchar();
	while ( isdigit(look->c) ) {
		t->tok[i++] = look->c; 
		t->val = ( 10 * t->val ) + (look->c - '0');
		look->c = getchar();
	}
	t->tok[i] = '\0';
	t->id = T_NUM;
}

void get_ident(struct Token *t, struct Look *look)
{
	int i;
	i = 0;
	t->tok[i++] = look->c;
	t->val = 0;
	look->c = getchar();
	while ( isalpha(look->c) ) {
		t->tok[i++] = look->c;
		look->c = getchar();
	}
	t->tok[i] = '\0';
	t->id = T_IDENT;
}

void get_oper(struct Token *t, struct Look *look)
{
	t->tok[0] = look->c;
	t->tok[1] = '\0';
	t->val = 0;
	printf("get_oper: c = \"%c\"\n", look->c);
	switch (look->c) {
	case '+' : t->id = T_ADD; break;
	case '*' : t->id = T_MUL; break;
	default  : t->id = T_NONE; break;
	}
	look->c = getchar();
}

int get_token(struct Token *token, struct Look *look)
{
	if ( look->c == EOF ) {
		return 1;
	}

	if ( isdigit(look->c) ) {
		get_number(token, look);
	} else if ( isalpha(look->c) ) {
		get_ident(token, look);
	} else {
		get_oper(token, look);
	}
	skip_white(look);
	return 0;
}

void scanner_init(struct Token *t, struct Look *l)
{
	t->val = 0;
	t->tok[0] = '\0';
	t->id = T_NONE;
	
	l->c = getchar();
	skip_white(l);
	//assert( get_token(t,l) == 0 );	
	
}

void print_token(struct Token *t)
{
        printf("STRUCT:\n");
	printf(" text = \"%s\"\n", t->tok);
        printf("  val = %d\n", t->val);
	printf("   id = %d\n", t->id);
}
