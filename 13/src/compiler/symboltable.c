//
// Symbol table using hashmaps from Learn C the Hard Way
//  The hash table is modified to take integers as the values for each key
//

typedef struct SymbolTable {
	DArray *identifiers;  // store the identifiers as bstrings
	Hashmap *tbl;  // nodes will point to a string in identifiers, and store and int
	int num_symbols;
} SymbolTable;

SymbolTable *SymTable;

/* Print out the symbol table for debug */
int traverse_cb(HashmapNode *node)
{
	check_mem(node);
	log_info("KEY: %s = %d", bdata( (bstring)node->key ), node->data );
	return 0;
error:
	return 1;
}

void ShowSymTable(void)
{
	int rc;
	log_info("SYMBOL TABLE:");
	rc = Hashmap_traverse(SymTable->tbl, traverse_cb);
	if (rc==1) {
		log_err("Something went wrong on symbol table printout");
	}
}

/* Symbol table operations */
SymbolTable *Symtable_create(void)
{
	SymbolTable *symtbl = calloc(1, sizeof(SymbolTable));
	symtbl->tbl = Hashmap_create(NULL, NULL);
	symtbl->identifiers = DArray_create(sizeof(bstring), MAXENTRY);
	symtbl->num_symbols = 0;
	return symtbl;
}

void Symtable_insert(SymbolTable *symtbl, char *ident, int val)
{
	int rc;
	bstring b_ident = DArray_new(symtbl->identifiers);
	b_ident = bfromcstr(ident);
	rc = Hashmap_set(symtbl->tbl, b_ident, val);
	if (rc==-1) {
		log_err("Can't insert into symboltable");
	}
	//free(b);
}

int Symtable_get(SymbolTable *symtbl, char *ident)
{
	bstring b_ident = bfromcstr(ident);
	return Hashmap_get(symtbl->tbl, b_ident);
}

/* returns 0 if symbol is not in the table, otherwise it returns
the integer associated with the key */
int Symtable_exists(SymbolTable *symtbl, char *ident)
{
	int rc;
	bstring b_ident = bfromcstr(ident);
	//ShowSymTable();
	rc = Hashmap_get(symtbl->tbl, b_ident);
	if (rc == -1)
		return 0;
	return 1;
}

void Symtable_destroy(SymbolTable *symtbl)
{
	Hashmap_destroy(symtbl->tbl);
	DArray_destroy(symtbl->identifiers);
	free(symtbl);
	symtbl = NULL;	
}

/* Wrapper functions for new hashmap-based symbol table */
int InTable(char *ident)
{
	return Symtable_exists(SymTable, ident);
}

void AddEntry(char *ident)
{
	char msg[MAXMSG];
	//ShowSymTable();
	if ( InTable(ident) ) {
		snprintf(msg, MAXMSG,
		    "Dupe variable declaration : \'%s\'", ident);
		Abort(msg);
	}

	if ( SymTable->num_symbols == MAXENTRY ) {
		Abort("Symbol table full");
	}
	
	Symtable_insert(SymTable, ident, 1);
	SymTable->num_symbols++;
}

