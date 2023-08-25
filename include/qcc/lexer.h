#ifndef LEXER_H_
#define LEXER_H_

#include "qcc/hashmap.h"
#include "qcc/list.h"
#include "qcc/token.h"
#include "qcc/macro.h"
#include "qcc/source.h"

#include "qcc/config.h"

typedef struct lexer Lexer;
struct lexer {
  List* sources;
  Hashmap* keywords;
  Hashmap* macros;
  List* cache;
};

/* constructor/destructor methods for lexer */
Lexer* lexer_create(List* sources);
void lexer_destroy(Lexer* lexer);

/* return a token and move up pointer */
Token* lexer_get(Lexer* lexer);

/* return token but dont move pointer */
Token* lexer_peek(Lexer* lexer);

/* move pointer and dont return token, returns the increment of pointer */
int lexer_eat(Lexer* lexer);

#ifdef QCC_DEBUG
/* dump state of lexer */
void lexer_dump(Lexer* lexer);
#endif // QCC_DEBUG
#endif // LEXER_H_
