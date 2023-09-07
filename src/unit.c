#include <stdlib.h>
#include "qcc/unit.h"
#include "qcc/logger.h"

Unit* unit_create(const char* path) {
  Unit* unit = (Unit *)malloc(sizeof(Unit));
  if (!(unit->file = source_create(path)))
    logger_fatal(-1, "Could not open file at path: %s\n", path);

  unit->lexer = lexer_create();
  if (lexer_register(unit->lexer, unit->file) == -1)
    logger_fatal(-1, "Could not register source at path: %s\n", path);

  unit->table = symtab_create();
  unit->parser = parser_create(unit->lexer, unit->table);
  //TODO: more init
  return unit;
}

void unit_destroy(Unit* unit) {}

int unit_compile(Unit* unit) {
  unit->tree = parser_run(unit->parser);
  ast_dump(unit->tree, 0);
}
