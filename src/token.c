#include <stdlib.h>
#include <string.h>

#include "qcc/token.h"

Token* token_create(int kind, char* loc, int length, void* value) {
  Token* token = malloc(sizeof(Token));
  token->kind = kind;
  token->loc = loc;
  token->length = length;
  token->value = value;
  return token;
}

void token_destroy(Token* token) {
  if (token)
    free(token);
  return;
}

int token_compare(Token* token, const char* string) {
  return strncmp(token->loc, string, token->length);
}

#ifdef QCC_DEBUG
void token_dump(Token* token) {

  return;
}
#endif // QCC_DEBUG
