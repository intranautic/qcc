#ifndef PARSER_H_
#define PARSER_H_

#include "qcc/token.h"
#include "qcc/lexer.h"

#include "qcc/ast.h"
#include "qcc/type.h"

#include "qcc/symbol.h"

typedef struct parser Parser;
struct parser {
  Lexer* lexer;
  Symtab* tabref;
};

/* constructor/destructor methods for parser state */
Parser* parser_create(Lexer* lexer, Symtab* tabref);
void parser_destroy(Parser* parser);
/* api to run parser on given translation unit */
Node* parser_run(Parser* parser);
#endif // PARSER_H_
