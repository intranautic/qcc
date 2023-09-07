#include <stdlib.h>
#include "qcc/unit.h"
#include "qcc/logger.h"

Unit* unit_create(const char* path) {
  Unit* unit = (Unit *)malloc(sizeof(Unit));
  unit->lexer = lexer_create();
  if (lexer_register(unit->lexer, path) == -1)
    logger_fatal(-1, "Could not register unit at path: %s\n", path);
  unit->table = symtab_create();
  unit->parser = parser_create(unit->lexer, unit->table);
  //TODO: more init
  return unit;
}

void unit_destroy(Unit* unit) {}

int unit_compile(Unit* unit) {

}
