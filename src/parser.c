#include <stdlib.h>
#include <string.h>

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
// statement:
//  ID: statement
//  case constant-expression : statement
//  default: statement
//  [ expression ] ;
//  if ( expression ) statement
//  if ( expression ) statement else statement
//  switch ( expression ) statement
//  while ( expression ) statement
//  do statement while ( expression )
//  for ( [ expression ] ; [ expression ] ; [ expression ] )
//    statement
//  break;
//  continue;
//  goto ID;
//  return [ expression];
//  compound-statement
static Node* parse_stmt(Parser* parser) {
}

// primary-expression:
//   identifer
//   constant
//   string-literal
//   '(' expression ')' 
static Node* parse_primary_expr(Parser* parser) {
  Token* tok;
  Node* node;
  if (tok = lexer_peek(parser->lexer)) {
    switch (tok->kind) {
      // TODO: implement string lexing and reloc static literal string type
      case TOKEN_LSTRING: break;
      // TODO: perform lookup in symbol table once decl and types are done
      case TOKEN_IDENTIFIER:
        node = INIT_ALLOC(Node, {
          .kind = EXPR_IDENT,
          .ident = lexer_get(parser->lexer)
        });
        break;
      case TOKEN_LCHAR:
        node = INIT_ALLOC(Node, {
          .kind = EXPR_CONST,
          .v.type = pred_char,
          .v.value = lexer_get(parser->lexer)
        });
        break;
      case TOKEN_LINTEGER:
        node = INIT_ALLOC(Node, {
          .kind = EXPR_CONST,
          .v.type = pred_long,
          .v.value = lexer_get(parser->lexer)
        });
        break;
      case TOKEN_LFLOAT:
        node = INIT_ALLOC(Node, {
          .kind = EXPR_CONST,
          .v.type = pred_double,
          .v.value = lexer_get(parser->lexer)
        });
        break;
      case TOKEN_LPAREN:
        lexer_eat(parser->lexer);
        node = parse_expr(parser);
        Token* err = lexer_get(parser->lexer);
        if (!err || err->kind != TOKEN_RPAREN)
          logger_fatal(-1, "Unclosed parenthesis on line %d\n", tok->line);
        break;
      default: return NULL;
    }
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
  Token* tok;
  if (tok = lexer_peek(parser->lexer)) {
    switch (tok->kind) {
      // subscript operation
      case TOKEN_LBRACKET:
        node = INIT_ALLOC(Node, {
          .kind = EXPR_POSTFIX,
          .e.op = lexer_get(parser->lexer),
          .e.lhs = node,
          .e.rhs = parse_expr(parser)
        });
        tok = lexer_get(parser->lexer);
        if (!tok || tok->kind != TOKEN_RBRACKET) {
          logger_fatal(-1, "Unclosed subscript on line %d\n",
            tok->line);
        }
        free(tok);
        break;
      // function call operation
      // TODO:
      case TOKEN_LPAREN:
        free(lexer_get(parser->lexer));
        node = INIT_ALLOC(Node, {
          .kind = EXPR_CALL,
          .f.name = node,
          .f.args = parse_expr(parser)
        });
        break;
      case TOKEN_DOT:
      case TOKEN_ARROW:
        node = INIT_ALLOC(Node, {
          .kind = EXPR_POSTFIX,
          .e.op = lexer_get(parser->lexer),
          .e.lhs = node,
          .e.rhs = parse_primary_expr(parser)
        });
        if (node->e.rhs->e.op->kind != TOKEN_IDENTIFIER) {
          logger_fatal(-1, "Member access not an identifier on line %d\n",
            node->e.rhs->e.op->line);
        }
        break;
      case TOKEN_INC:
      case TOKEN_DEC:
        node = INIT_ALLOC(Node, {
          .kind = EXPR_POSTFIX,
          .e.op = lexer_get(parser->lexer),
          .e.lhs = node
        });
        break;
      default: break;
    }
  }
  return node;
}

// unary-expression:
//   postfix-expression
//   unary-operator unary-expression
//   '(' type-name ')' unary-expression
//   sizeof unary-expression
//   sizeof ' ( ' type-name ' ) '
// unary-operator:
//   one of ++ -- & * + - ~ !
// TODO: typecast (needs declspec and type parsing)
// TODO: implement sizeof unary operator parsing
static Node* parse_unary_expr(Parser* parser) {
  Node* node;
  Token* tok;
  if (tok = lexer_peek(parser->lexer)) {
    switch (tok->kind) {
      case TOKEN_INC:
      case TOKEN_DEC:
      // reference
      case TOKEN_BAND:
      // dereference
      case TOKEN_MUL:
      case TOKEN_ADD:
      case TOKEN_SUB:
      case TOKEN_BNOT:
        node = INIT_ALLOC(Node, {
          .kind = EXPR_UNARY,
          .e.op = lexer_get(parser->lexer),
          .e.lhs = parse_unary_expr(parser)
        });
        break;
      default:
        node = parse_postfix_expr(parser);
        break;
    }
  }
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
        node = INIT_ALLOC(Node, {
          .kind = EXPR_BINARY,
          .e.op = lexer_get(parser->lexer),
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
        node = INIT_ALLOC(Node, {
          .kind = EXPR_BINARY,
          .e.op = lexer_get(parser->lexer),
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
        node = INIT_ALLOC(Node, {
          .kind = EXPR_BINARY,
          .e.op = lexer_get(parser->lexer),
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
        node = INIT_ALLOC(Node, {
          .kind = EXPR_BINARY,
          .e.op = lexer_get(parser->lexer),
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
        node = INIT_ALLOC(Node, {
          .kind = EXPR_BINARY,
          .e.op = lexer_get(parser->lexer),
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
  if (tok = lexer_peek(parser->lexer)) {
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
          .e.op = lexer_get(parser->lexer),
          .e.lhs = node,
          .e.rhs = parse_assign_expr(parser)
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
  //TODO: broken
  while (tok = lexer_peek(parser->lexer)) {
    if (tok->kind == TOKEN_COMMA) {
      node = INIT_ALLOC(Node, {
        .kind = EXPR_BINARY,
        .e.op = lexer_get(parser->lexer),
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
