#ifndef AST_H_
#define AST_H_

#include "qcc/symbol.h"
#include "qcc/token.h"
#include "qcc/type.h"

typedef struct node Node;
struct node {
  enum {
    // type node
    NODE_TYPE,
    // expression
    NODE_ASSIGN,
    NODE_BINARY,
    NODE_UNARY,
    NODE_POSTFIX,
    // special
    NODE_CALL,
    NODE_TYPECAST,
    // conditional
    NODE_TERNARY,
    // primary expressions
    NODE_CONST,
    NODE_STRING,
    NODE_IDENT,
    // statement
    NODE_BLOCK,
    NODE_LABEL,
    NODE_EXPR,
    NODE_IF,
    NODE_ELSE,
    NODE_WHILE,
    NODE_FOR,
    NODE_DO,
    NODE_SWITCH,
    NODE_CASE,
    NODE_DEFAULT,
    NODE_GOTO,
    NODE_CONTINUE,
    NODE_BREAK,
    NODE_RETURN,
    // declarations
    NODE_ENUM,
    NODE_STRUCT,
    NODE_UNION,
    NODE_ARRAY,
    NODE_VARIABLE,
    NODE_FUNCTION,
    // initializer
    NODE_INITIALIZER
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
