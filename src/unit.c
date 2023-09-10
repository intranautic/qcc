#include <stdlib.h>
#include "qcc/unit.h"
#include "qcc/logger.h"

Unit* unit_create(void) {
  Unit* unit = (Unit *)calloc(1, sizeof(Unit));
  unit->lexer = lexer_create();
  unit->table = symtab_create();
  unit->parser = parser_create(unit->lexer, unit->table);
  //TODO: more init
  return unit;
}

void unit_destroy(Unit* unit) {
  if (!unit)
    return;

  symtab_destroy(unit->table);
  lexer_destroy(unit->lexer);
  parser_destroy(unit->parser);
  ast_destroy(unit->tree);
  free(unit);
  return;
}

int unit_register(Unit* unit, const char* file) {
  unit->file = file;
  return lexer_register(unit->lexer, source_create(unit->file));
}

int unit_compile(Unit* unit) {
  unit->tree = parser_run(unit->parser);
  ast_dump(unit->tree, 0);
}
