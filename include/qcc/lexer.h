#ifndef LEXER_H_
#define LEXER_H_

#include "qcc/list.h"
#include "qcc/token.h"
#include "qcc/macro.h"
#include "qcc/source.h"
#include "qcc/logger.h"

typedef struct lexer Lexer;
struct lexer {
  List* sources;
  Hashmap* keywords;
  Hashmap* types;

  /*
    preprocessor:
    macros: maps H(name) -> list of preprocessed tokens
    cache: list of macro tokens cached due to incremental lexing
  */
  Hashmap* macros;
  List* cache;
};

/* constructor/destructor methods for lexer */
Lexer* lexer_create(List* sources);
void lexer_destroy(Lexer* lexer);

/* return a token and move up pointer */
Token* lexer_get(Lexer* lexer, Source* source);

/* return token but dont move pointer */
Token* lexer_peek(Lexer* lexer, Source* source);

/* move pointer and dont return token, returns the increment of pointer */
int lexer_eat(Lexer* lexer, Source* source);

/* handling of preprocessor directives */
List* lexer_directive(Lexer* lexer, Source* source);

/* register source to top of source list */
int lexer_include(const char* path);
#endif // LEXER_H_
