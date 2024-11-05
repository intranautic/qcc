#ifndef AST_H_
#define AST_H_

#include "qcc/symbol.h"
#include "qcc/token.h"
#include "qcc/type.h"

typedef struct node Node;
struct node {
  enum {
    // root node
    NODE_PROGRAM,
    // type node
    NODE_TYPE,
    // expr arithmetic
    NODE_BINARY,
    NODE_UNARY,
    NODE_POSTFIX,
    // expr operation
    NODE_ASSIGN,
    NODE_ACCESS,
    NODE_CALL,
    NODE_SUBSCRIPT,
    NODE_TYPECAST,
    NODE_SIZEOF,
    // conditional
    NODE_TERNARY,
    // primary expressions
    NODE_CONST,
    NODE_STRING,
    NODE_IDENT,
    // statement
    NODE_LABEL,
    NODE_BLOCK,
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
    NODE_DECL,
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
    // function call
    struct {
      Node* name;
      Node* args;
    } f;
    // constant value (no symbol)
    struct {
      Type* type;
      Token* value;
    } v;
    // control
    struct {
      Node* cond;
      Node* ifnode;
      Node* elnode;
    } c;
    // symbol
    struct {
      Symbol* symbol;
      Node* body;
    } s;
    // declarator
    struct {
      Token* spec;
      void* init; // TODO: initializer list parsing
    } d;
    Token* label;
  };
  Node* next;
};

/* recursively dump tree */
void ast_dump(Node* root, int depth);
/* recursively destroy tree */
void ast_destroy(Node* root);

/* premature optimization on ast */
void ast_fold(Node* root);
#endif // AST_H_
