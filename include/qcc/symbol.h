#ifndef SYMBOL_H_
#define SYMBOL_H_

#include "qcc/token.h"
#include "qcc/type.h"
#include "qcc/hashmap.h"
#include "qcc/list.h"

typedef struct symbol Symbol;
typedef struct scope Scope;
typedef struct symtab Symtab;
struct symbol {
  enum {
    STORE_TYPEDEF,
    STORE_EXTERN,
    STORE_STATIC,
    STORE_AUTO,
    STORE_REGISTER
  } storage;
  Type* type;
  Token* ident;
  Scope* scope;
};

struct scope {
  Hashmap* lookup;
  Scope* upref;
  List* subscope;
};

struct symtab {
  // struct/union/enum tag or file scope
  Scope* tag;
  // global/local scopes and nested block scopes
  Scope* global;
  Scope* current;
};

/* symbol constructor/destructor methods */
Symbol* symbol_create(int store, Type* type, Token* ident);
void symbol_destroy(Symbol* symbol);

/* symbol table constructor/destructor methods */
Symtab* symtab_create(void);
void symtab_destroy(Symtab* table);

/* scope enter/leave operations */
void symtab_enter(Symtab* table);
void symtab_leave(Symtab* table);

/* install a new symbol into the current scope of the symbol table */
int symtab_install(Symtab* table, Symbol* symbol);

/* remove symbol from table */
int symtab_remove(Symtab* table, Symbol* symbol);

/* symbol has ref to scope, use for type-checking in ast */
Symbol* scope_lookup(Scope* scope, Token* ident);
#endif // SYMBOL_H_
