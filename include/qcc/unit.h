#ifndef UNIT_H_
#define UNIT_H_

#include "qcc/config.h"

#include "qcc/symbol.h"
#include "qcc/source.h"
#include "qcc/lexer.h"
#include "qcc/ast.h"
#include "qcc/parser.h"

#include "qcc/llir.h"
#include "qcc/cfg.h"
#include "qcc/ssa.h"

#include "qcc/optimize.h"

typedef struct unit Unit;
struct unit {
  Config config;
  Symtab* table;
  Lexer* lexer;
  Parser* parser;
  Node* tree;
  LLIR* llir;
  LLIR* llir_ssa;
  CFG* cfg;
  Optreg* optimize;
};

Unit* unit_create(const char* path);
void unit_destroy(Unit* unit);

int unit_compile(Unit* unit);
#endif // UNIT_H_
