#ifndef AST_H_
#define AST_H_

#include "qcc/symbol.h"
#include "qcc/token.h"
#include "qcc/type.h"

typedef struct node Node;
struct node {
  enum {
    // type node
    EXPR_TYPE,
    // expression
    EXPR_ASSIGN,
    EXPR_BINARY,
    EXPR_UNARY,
    EXPR_POSTFIX,
    // special
    EXPR_CALL,
    EXPR_TYPECAST,
    // conditional
    EXPR_TERNARY,
    // primary expressions
    EXPR_CONST,
    EXPR_STRING,
    EXPR_IDENT,
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
    // expr
    struct {
      Token* op;
      Node* lhs;
      Node* rhs;
    } e;
    // control
    struct {
      Node* cond;
      Node* ifnode;
      Node* elnode;
    } c;
    // func
    struct {
      Node* name;
      Node* args;
    } f;
    // constant value (no symbol)
    struct {
      Type* type;
      Token* value;
    } v;
    // statement
    struct {
      Node* expr;
      Node* next;
    } s;
    // reference to declaration
    Symbol* decl;
    Token* ident;
  };
};

/* recursively dump tree */
void ast_dump(Node* root, int depth);
/* recursively destroy tree */
void ast_destroy(Node* root);

/* premature optimization on ast */
int ast_constfold(Node* root);
#endif // AST_H_
