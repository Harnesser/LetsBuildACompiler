//
// Symbol table using hashmaps from Learn C the Hard Way
//  The hash table is modified to take integers as the values for each key
//

typedef enum { SYM_NONE, SYM_VAR=10, SYM_FUNC=100 } sym_type_e;

typedef struct SymbolTable {
	DArray *identifiers;  // store the identifiers as bstrings
	Hashmap *tbl;  // nodes will point to a string in identifiers, and store and int
	int num_symbols;
	int num_args;
	int num_locals;
} SymbolTable;

SymbolTable *SymTable;
SymbolTable *LocalSymTable;

/* Print out the symbol table for debug */
int traverse_cb(HashmapNode *node)
{
	check_mem(node);
	log_info("KEY: %s = %d", bdata( (bstring)node->key ), node->data );
	return 0;
error:
	return 1;
}

void Symtable_show(SymbolTable *symtbl)
{
	int rc;
	if (symtbl == NULL) {
		log_info("(empty)");
		return;
	}
	rc = Hashmap_traverse(symtbl->tbl, traverse_cb);
	if (rc==1) {
		log_err("Something went wrong on symbol table printout");
	}
}

/* Symbol table operations */
SymbolTable *Symtable_create(void)
{
	SymbolTable *symtbl = calloc(1, sizeof(SymbolTable));
	symtbl->tbl = Hashmap_create(NULL, NULL);
	check_mem(symtbl->tbl);
	symtbl->identifiers = DArray_create(sizeof(bstring), MAXENTRY);
	check_mem(symtbl->identifiers);
	symtbl->num_symbols = 0;
	symtbl->num_locals = 0;
	symtbl->num_args = 0;
	return symtbl;

error:
	if(symtbl->tbl) {
		Hashmap_destroy(symtbl->tbl);
	}
	if(symtbl->identifiers) {
		DArray_destroy(symtbl->identifiers);
	}
	Abort("Failed to allocate mem for symbol table");
	return NULL;
}

void Symtable_insert(SymbolTable *symtbl, char *ident, int val)
{
	char msg[MAXMSG];
	int rc;
	bstring b_ident = bfromcstr(ident);

	// check we aren't inserting a symbol more than once
	if ( Hashmap_get(symtbl->tbl, b_ident) != -1 ) {
		snprintf(msg, MAXMSG, "Dupe variable declaration : \'%s\'", ident);
		Abort(msg);
	}

	// have we maxed out the symbol table?
	if (symtbl->num_symbols == MAXENTRY) {
		Abort("Symbol Table Full.");
	}

	// store the identifier string
	bstring b = DArray_new(symtbl->identifiers);
	b = bfromcstr(ident);

	// insert the symbol into the table
	rc = Hashmap_set(symtbl->tbl, b, val);
	if (rc==-1) {
		log_err("Can't insert into symboltable");
	}

	// update symbol counts
	symtbl->num_symbols++;
	if (val >= SYM_FUNC ) {
		symtbl->num_args++;
	} else if (val >= SYM_VAR) {
		symtbl->num_locals++;
	}

	// cleanup
	//free(b_ident);
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
	//symtbl = NULL; // this doesn't clear the calling pointer.
}

/* Wrapper functions for new hashmap-based symbol table */
int InTable(char *ident)
{
	return Symtable_exists(SymTable, ident);
}

int IsProcedure(char *ident)
{
	/* >= here cos we're keeping argument counts here too.
           arg count = _get - SYM_FUNC */
	return Symtable_get(SymTable, ident) >= SYM_FUNC;
}

void AddEntry(char *ident)
{
	Symtable_insert(SymTable, ident, SYM_VAR);
}

/* add an entry in the local symbol table for the variable.
 encode the index of it in the hashmap so we have a handle on it when
 we need to find it's place on the stack.
*/
void AddLocalEntry(char *ident)
{
	int var_index;

	if (LocalSymTable == NULL) {
		Abort("No local symbol table present!");
	}

	var_index = SYM_VAR + LocalSymTable->num_locals + 1;
	message("var_index = %d", var_index);
	Symtable_insert(LocalSymTable, ident, var_index);
}

/* look up the main symboltable and extract the number of
 args the function has. Returns -1 if failed */
int GetNumProcArgs(char *ident)
{
	int exists = Symtable_exists(SymTable, ident);
	if (exists == 0) {
		Abort("Expected to find function in main symbol table");
		return -1;
	}

	return Symtable_get(SymTable, ident) - SYM_FUNC;
}

/* return the stack pointer offset to get at the variable:
  negative : it's a local var
      0    : it's a global
  positive : it's a proc argument
*/
int GetOffset(char *ident)
{
	int offset = 0;

	if (LocalSymTable == NULL ) {
		return 0;
	}

	if (Symtable_exists(LocalSymTable, ident) == 0) {
		message(" %s not found in local symbol table", ident);
		return 0;
	}

	offset = Symtable_get(LocalSymTable, ident);
	message("Offset: %d", offset);
	if (offset == 0) {
		Abort("Identifier is unexpectedly SYM_NONE");
	} else if (offset > SYM_FUNC) {
		offset -= SYM_FUNC;
	} else {
		offset -= SYM_VAR;
		offset = -offset;
	}
	return offset;
}
