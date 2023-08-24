#include <stdlib.h>
#include <stdbool.h>

#include "qcc/symbol.h"
#include "qcc/token.h"
#include "qcc/type.h"
#include "qcc/hashmap.h"
#include "qcc/list.h"

struct symbol {
  Token* ident;
  Scope* scope;
  Type* type;
};

struct scope {
  int size; // if empty dont perform lookup
  bool is_leave; // if set allocate new scope from parent
  Hashmap* lookup;
  Scope* parent;
  List* succ;
};


