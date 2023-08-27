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
  Node* root;
};

//TODO:
Parser* parser_create(void);
void parser_destroy(Parser* parser);

Parser* parser_run(Parser* parser);
#endif // PARSER_H_
