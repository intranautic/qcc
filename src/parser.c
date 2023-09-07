#include <stdlib.h>

#include "qcc/parser.h"
#include "qcc/logger.h"
#include "qcc/initalloc.h"

/* --- statement parser --- */
static Node* parse_stmt(Parser* parser) {}
/* --- declaration parser --- */
static Node* parse_decl(Parser* parser) {}
/* --- expression parser --- */

static Node* parse_expr(Parser* parser);
static Node* parse_primary_expr(Parser* parser) {
  Token* tok = lexer_get(parser->lexer);
  Node* node;
  switch (tok->kind) {
    case TOKEN_IDENTIFIER:
      node = INIT_ALLOC(Node, {
        .kind = EXPR_IDENT,
        .token = tok,
        .next = NULL
      });
      break;
    case TOKEN_LCHAR:
      node = INIT_ALLOC(Node, {
        .kind = EXPR_CONST,
        .v.type = pred_char,
        .v.value = tok
      });
      break;
    //TODO: implement string lexing and const (reloc)
    case TOKEN_LSTRING: break;
    case TOKEN_LINTEGER:
      node = INIT_ALLOC(Node, {
        .kind = EXPR_CONST,
        .v.type = pred_long,
        .v.value = tok
      });
      break;
    case TOKEN_LFLOAT:
      node = INIT_ALLOC(Node, {
        .kind = EXPR_CONST,
        .v.type = pred_double,
        .v.value = tok
      });
      break;
    case TOKEN_LPAREN:
      node = parse_expr(parser);
      if (lexer_kget(parser->lexer) == TOKEN_RPAREN)
        return node;
      //TODO: implement error if not closed expr
    default: return NULL;
  }
  return node;
}

static Node* parse_postfix_expr(Parser* parser) {}
static Node* parse_unary_expr(Parser* parser) {}
static Node* parse_binary_expr(Parser* parser) {}
static Node* parse_cond_expr(Parser* parser) {}
static Node* parse_assign_expr(Parser* parser) {}

static Node* parse_expr(Parser* parser) {
  Node* node = parse_assign_expr(parser);
  Token* tok;
  while (tok = lexer_peek(parser->lexer)) {
    if (tok->kind == TOKEN_COMMA) {
      node = INIT_ALLOC(Node, {
        .kind = EXPR_COMMA,
        .e.op = tok,
        .e.lhs = node,
        .e.rhs = parse_assign_expr(parser)
      });
      lexer_eat(parser->lexer);
    } else
      break;
  }
  return node;
}

/* --- public parser api --- */
Parser* parser_create(Lexer* lexer, Symtab* tabref) {
  Parser* parser = (Parser *)malloc(sizeof(Parser));
  parser->lexer = lexer;
  parser->tabref = tabref;
  return parser;
}

void parser_destroy(Parser* parser) {
  if (parser)
    free(parser);
}

Node* parser_run(Parser* parser) {}
