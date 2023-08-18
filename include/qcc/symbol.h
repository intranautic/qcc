#ifndef SYMBOL_H_
#define SYMBOL_H_

#include "qcc/token.h"
#include "qcc/type.h"
#include "qcc/hashmap.h"

/*
  have a single global scope and pass around scopes to the same linked list.
  each reference will be able to access the scopes, but regardless they will
  have the same scope affect like that of a stack as long as they traverse
  downwards.

  every function has a symbol table (local)
  global, extern, static, const all have their own symbol tables
*/
typedef struct symbol Symbol;
typedef struct symtab Symtab;

struct symbol {
  enum {
    SYM_CONST,
    SYM_STATIC,
    SYM_EXTERN,
    SYM_GLOBAL,
    SYM_IDENT,
    SYM_TYPE
  } kind;
  int scope;
  Type type;
  Token* token; // token containing name
  Symtab* table; // symbol table which symbol belongs to
};

struct symtab {
  // current scope, we lazily allocate scopes so we need to keep track
  int current;
  Hashmap* symbols;
  Symtab* up; // up scope
  Symtab* down; // down scope
  /*
    we need to handle this case:
    {int x;}
    {x = 1;} // error!
    next scope prevents clashing scopes at the same level, new scope
    if down != 0, traverse down->next and work with that scope.
  */
  Symtab* next;
};

/* construction and destruction methods for symtab */
Symtab* symtab_create(void);
void symtab_destroy(Symtab* symtab);

/* traverses up prev scopes and searches each hashmap for identifier */
Symbol* symtab_search(Symtab* symtab);

/* insert symbol into symbol table */
Symbol* symtab_insert(Symtab* symtab, Symbol* symbol);

/* enter and leaves scope */
Symtab* symtab_enter(Symtab* symtab);
Symtab* symtab_leave(Symtab* symtab);
#endif // SYMBOL_H_
