#ifndef SYMBOL_H_
#define SYMBOL_H_

#include "qcc/token.h"
#include "qcc/type.h"
#include "qcc/hashmap.h"
#include "qcc/list.h"

typedef struct symbol Symbol;
typedef struct symtab Scope;

struct symbol {
  Token* ident;
  Scope* scope;
  Type type;
};

struct scope {
  // table of symbols per scope
  Hashmap* lookup;
  // parent scope
  Scope* parent;
  // list of successor/nested scopes
  List* succ;
};

typedef struct symtab Symtab;
struct symtab {
  Scope* root;
  Scope* current;
};

#endif // SYMBOL_H_
