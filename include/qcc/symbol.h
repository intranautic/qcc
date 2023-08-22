#ifndef SYMBOL_H_
#define SYMBOL_H_

typedef struct symbol Symbol;
typedef struct symtab Scope;

typedef struct symtab Symtab;
struct symtab {
  Scope* root;
  Scope* current;
};

/* symbol table constructor/destructor methods */
Symtab* symtab_create(void);
void symtab_destroy(Symtab* table);

/* scope enter/leave operations */
int symtab_enter(Symtab* table);
int symtab_leave(Symtab* table);

/* install a new symbol into the current scope of the symbol table */
int symtab_install(Symtab* table, Symbol* symbol);

/* symbol has ref to scope, use for type-checking in ast */
Symbol* scope_lookup(Scope* scope);
#endif // SYMBOL_H_
