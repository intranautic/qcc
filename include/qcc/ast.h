#ifndef AST_H_
#define AST_H_

#include "qcc/token.h"

typedef struct node Node;
struct node {
  enum {
    // expression
    EXPR_ASSIGN,
    EXPR_CONDITION,
    EXPR_TERNARY,
    EXPR_BINARY,
    EXPR_UNARY,
    EXPR_POSTFIX,
    // primary expressions
    EXPR_CONST,
    EXPR_IDENT,
    EXPR_STRING,
    // statement
    STMT_BLOCK,
    STMT_LABEL,
    STMT_EXPR,
    STMT_IF,
    STMT_ELSE,
    STMT_WHILE,
    STMT_FOR,
    STMT_DO,
    STMT_SWITCH,
    STMT_CASE,
    STMT_DEFAULT,
    STMT_GOTO,
    STMT_CONTINUE,
    STMT_BREAK,
    STMT_RETURN,
    // declarations
    DECL_ENUM,
    DECL_STRUCT,
    DECL_UNION,
    DECL_ARRAY,
    DECL_VARIABLE,
    DECL_FUNCTION,
    // initializer
    DECL_INITIALIZER
  } kind;
  union {
    struct {
      enum {
        //TODO: add more operators, binary unary and postfix
        OP_ADD,
      } op;
      Node* left;
      Node* right;
    } expr;
  };
  Node* next;
};

/* no constructor/destructor apis, use initalloc for complex types */
#endif // AST_H_
