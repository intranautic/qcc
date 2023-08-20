#ifndef SYMBOL_H_
#define SYMBOL_H_

typedef struct symbol Symbol;
typedef struct symtab Scope;

typedef struct symtab Symtab;
struct symtab {
  Scope* root;
  Scope* current;
};

Symtab* symtab_create(void);
void symtab_destroy(Symtab* table);

int symtab_enter(Symtab* table);
int symtab_leave(Symtab* table);
int symtab_install(Symtab* table, Symbol* symbol);

Symbol* scope_lookup(Scope* scope);
#endif // SYMBOL_H_
