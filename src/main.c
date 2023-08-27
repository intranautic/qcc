#include <stdio.h>
#include "qcc/qcc.h"

int main(int argc, const char** argv) {
  logger_init();
  logger_register("default", LOG_INFO, stderr);

  // one round per translation unit, realloc and dealloc each time
  for (int i = 1; i < argc; ++i) {
    Lexer* lexer = lexer_create();
    if (lexer_register(lexer, argv[i]) != -1)
      logger_info("Registered unit %s in lexer!\n", argv[i]);

    logger_info("Starting tokenization loop\n");
    Token* tok;
    while (tok = lexer_get(lexer)) {
      logger_info(TOKEN_DEBUG(tok));
      token_destroy(tok);
    }
    lexer_destroy(lexer);
  }

  logger_destroy();
  return 0;
}
