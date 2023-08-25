#include <stdio.h>
#include "qcc/qcc.h"

int main(int argc, const char** argv) {
  logger_init();
  logger_register("default", LOG_INFO, stderr);

  if (argv[1]) {
    List* sources = list_create(source_create(argv[1]));
    source_fill((Source *)sources->value);
    Lexer* lexer = lexer_create(sources);
    Token* tok = lexer_get(lexer);
    token_dump(tok);
    lexer_destroy(lexer);
    source_destroy((Source *)sources->value);
    list_destroy(sources);
  } else
    logger_error("Usage: %s <path>\n", argv[0]);
  logger_destroy();
  return 0;
}
