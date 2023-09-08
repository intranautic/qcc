#include <stdlib.h>

#include "qcc/parser.h"
#include "qcc/logger.h"
#include "qcc/initalloc.h"

/* --- statement parser signatures --- */
static Node* parse_stmt(Parser* parser);
/* --- declaration parser signatures --- */
static Node* parse_decl(Parser* parser);
/* --- expression parser signatures --- */
static Node* parse_primary_expr(Parser* parser);
static Node* parse_postfix_expr(Parser* parser);
static Node* parse_unary_expr(Parser* parser);
static Node* parse_binary_expr(Parser* parser);
static Node* parse_cond_expr(Parser* parser);
static Node* parse_assign_expr(Parser* parser);
static Node* parse_expr(Parser* parser);
/* --- parser implementation --- */

// primary-expression:
//   identifer
//   constant
//   string-literal
//   '(' expression ')' 
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
      tok = lexer_get(parser->lexer);
      if (!tok || tok->kind != TOKEN_RPAREN)
        logger_fatal(-1, "Unclosed parenthesis on line %d\n", tok->line);
      break;
    default: return NULL;
  }
  return node;
}

// postfix-expression:
//   primary-expression { postfix-operator }
// postfix-operator:
//   '[' expression ']'
//   '(' [ assignment-expression { , assignment-expression } ] ')'
//   . identifier
//   -> identifier
//   ++
//   --
static Node* parse_postfix_expr(Parser* parser) {
  Node* node = parse_primary_expr(parser);
  return node;
}

// unary-expression:
//   postfix-expression
//   unary-operator unary-expression
//   '(' type-name ')' unary-expression
//   sizeof unary-expression
//   sizeof ' ( ' type-name ' ) '
// unary-operator:
//   one of ++ -- & * + - - !
static Node* parse_unary_expr(Parser* parser) {
  Node* node = parse_postfix_expr(parser);
  return node;
}

// TODO: change this into an operator prec parser, this doesnt have precedence rn :(
// binary-expression:
//   unary-expression { binary-operator unary-expression }
// binary-operator:
//   one of || && | ^ & == != < > <= >= << >> + - * / %
static Node* parse_binary_expr2(Parser* parser) {
  Node* node = parse_unary_expr(parser);
  Token* tok;
  while (tok = lexer_peek(parser->lexer)) {
    switch (tok->kind) {
      case TOKEN_MUL:
      case TOKEN_DIV:
      case TOKEN_MOD:
        lexer_eat(parser->lexer);
        node = INIT_ALLOC(Node, {
          .kind = EXPR_BINARY,
          .e.op = tok,
          .e.lhs = node,
          .e.rhs = parse_unary_expr(parser)
        });
        break;
      default: return node;
    }
  }
  return node;
}

static Node* parse_binary_expr(Parser* parser) {
  Node* node = parse_binary_expr2(parser);
  Token* tok;
  while (tok = lexer_peek(parser->lexer)) {
    switch (tok->kind) {
      case TOKEN_ADD:
      case TOKEN_SUB:
        lexer_eat(parser->lexer);
        node = INIT_ALLOC(Node, {
          .kind = EXPR_BINARY,
          .e.op = tok,
          .e.lhs = node,
          .e.rhs = parse_binary_expr2(parser)
        });
        break;
      default: return node;
    }
  }
  return node;
}

static Node* parse_relational_expr(Parser* parser) {
  Node* node = parse_binary_expr(parser);
  Token* tok;
  while (tok = lexer_peek(parser->lexer)) {
    switch (tok->kind) {
      case TOKEN_EQ:
      case TOKEN_NE:
      case TOKEN_LT:
      case TOKEN_GT:
      case TOKEN_LTE:
      case TOKEN_GTE:
        lexer_eat(parser->lexer);
        node = INIT_ALLOC(Node, {
          .kind = EXPR_BINARY,
          .e.op = tok,
          .e.lhs = node,
          .e.rhs = parse_binary_expr(parser)
        });
        break;
      default: return node;
    }
  }
  return node;
}

static Node* parse_bitwise_expr(Parser* parser) {
  Node* node = parse_relational_expr(parser);
  Token* tok;
  while (tok = lexer_peek(parser->lexer)) {
    switch (tok->kind) {
      case TOKEN_BOR:
      case TOKEN_BAND:
      case TOKEN_BNOT:
      case TOKEN_BLSHIFT:
      case TOKEN_BRSHIFT:
        lexer_eat(parser->lexer);
        node = INIT_ALLOC(Node, {
          .kind = EXPR_BINARY,
          .e.op = tok,
          .e.lhs = node,
          .e.rhs = parse_relational_expr(parser)
        });
        break;
      default: return node;
    }
  }
  return node;
}

static Node* parse_logic_expr(Parser* parser) {
  Node* node = parse_bitwise_expr(parser);
  Token* tok;
  while (tok = lexer_peek(parser->lexer)) {
    switch (tok->kind) {
      case TOKEN_LOR:
      case TOKEN_LAND:
      case TOKEN_LNOT:
        lexer_eat(parser->lexer);
        node = INIT_ALLOC(Node, {
          .kind = EXPR_BINARY,
          .e.op = tok,
          .e.lhs = node,
          .e.rhs = parse_bitwise_expr(parser)
        });
        break;
      default: return node;
    }
  }
  return node;
}

// conditional-expression:
//   binary-expression [ ? expression : conditional-expression ]
static Node* parse_cond_expr(Parser* parser) {
  Node* node = parse_logic_expr(parser);
  Token* tok = lexer_peek(parser->lexer);
  if (tok && tok->kind == TOKEN_QUESTION) {
    lexer_eat(parser->lexer);

    Node* lhs = parse_expr(parser);
    tok = lexer_get(parser->lexer);
    if (!tok || tok->kind != TOKEN_COLON)
      logger_fatal(-1, "Invalid ternary expression at line %d\n", tok->line);

    node = INIT_ALLOC(Node, {
      .kind = EXPR_TERNARY,
      .c.cond = node,
      .c.ifnode = lhs,
      .c.elnode = parse_cond_expr(parser)
    });
  }
  return node;
}

// assignment-expression:
//   conditional-expression
//   unary-expression assign-operator assignment-expression
// assign-operator:
//   one of = += -= *= /= %= <<= >>= &= &= I=
static Node* parse_assign_expr(Parser* parser) {
  Node* node = parse_cond_expr(parser);
  Token* tok;
  while (tok = lexer_peek(parser->lexer)) {
    switch (tok->kind) {
      case TOKEN_ASSIGN:
      case TOKEN_ASGN_ADD:
      case TOKEN_ASGN_SUB:
      case TOKEN_ASGN_MUL:
      case TOKEN_ASGN_DIV:
      case TOKEN_ASGN_MOD:
      case TOKEN_ASGN_BAND:
      case TOKEN_ASGN_BOR:
      case TOKEN_ASGN_BXOR:
      case TOKEN_ASGN_BLSHIFT:
      case TOKEN_ASGN_BRSHIFT:
        lexer_eat(parser->lexer);
        node = INIT_ALLOC(Node, {
          .kind = EXPR_ASSIGN,
          .e.op = tok,
          .e.lhs = node,
          .e.rhs = parse_cond_expr(parser)
        });
        break;
      default: return node;
    }
  }
  return node;
}

// expression:
//   assignment-expression { , assignment-expression }
static Node* parse_expr(Parser* parser) {
  Node* node = parse_assign_expr(parser);
  Token* tok;
  while (tok = lexer_peek(parser->lexer)) {
    if (tok->kind == TOKEN_COMMA) {
      lexer_eat(parser->lexer);
      node = INIT_ALLOC(Node, {
        .kind = EXPR_BINARY,
        .e.op = tok,
        .e.lhs = node,
        .e.rhs = parse_assign_expr(parser)
      });
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

Node* parser_run(Parser* parser) {
  return parse_expr(parser);
}
