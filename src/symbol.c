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
  bool is_leave;
  Hashmap* lookup;
  Scope* parent;
  List* succ;
};

static Symbol* symbol_create();
static void symbol_destroy(Symbol* symbol);

static Scope* scope_create();
static void scope_destroy(Scope* scope);

