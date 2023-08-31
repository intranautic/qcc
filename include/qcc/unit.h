#ifndef UNIT_H_
#define UNIT_H_

#include "qcc/symbol.h"

typedef struct unit Unit;
struct unit {
  const char* path;
  Symtab* symbol;
};



#endif // UNIT_H_
