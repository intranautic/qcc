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
  Hashmap* keywords;
  Hashmap* macros;
  List* expand;
  List* sources;
};

/* constructor/destructor methods for lexer */
Lexer* lexer_create(void);
void lexer_destroy(Lexer* lexer);

/* register source for lexer */
int lexer_register(Lexer* lexer, Source* source);

/* return a token and move up pointer */
Token* lexer_get(Lexer* lexer);

/* return token but dont move pointer */
Token* lexer_peek(Lexer* lexer);

/* move pointer and dont return token, returns the increment of pointer */
int lexer_eat(Lexer* lexer);

/* only return the kind of token, free token so we dont need to manage in parser */
int lexer_kget(Lexer* lexer);
int lexer_kpeek(Lexer* lexer);
#endif // LEXER_H_
