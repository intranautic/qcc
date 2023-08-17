#ifndef SYMBOL_H_
#define SYMBOL_H_

#include "qcc/type.h"
#include "qcc/hashmap.h"

typedef struct symbol Symbol;
struct symbol {
  char* name;
  int scope;
  Type type;
};

typedef struct symtab Symtab;
struct symtab {
  Hashmap* symbols;
  int size;
};

#endif // SYMBOL_H_
