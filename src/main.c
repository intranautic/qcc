#include <stdio.h>
#include "qcc/qcc.h"

int main(int argc, const char** argv) {
  logger_init();
  logger_register("default", LOG_INFO, stderr);

  List* sources = NULL;
  Source* tmp;
  for (int i = 1; i < argc; ++i) {
    tmp = source_create(argv[i]);
    if (tmp) {
      logger_info("Registered source: %s\n", argv[i]);
      source_fill(tmp);
      list_fpush(&sources, tmp);
    }
  }

  Lexer* lexer = lexer_create(sources);
  Token* tok;
  logger_info("Starting tokenization loop\n");
  while (tok = lexer_get(lexer)) {
    logger_info(TOKEN_DEBUG(tok));
    token_destroy(tok);
  }

  lexer_destroy(lexer);
  logger_destroy();
  return 0;
}
