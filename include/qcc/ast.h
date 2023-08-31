#ifndef AST_H_
#define AST_H_

typedef struct node Node;
struct node {
  enum {
    NODE_
  } kind;
};

/* no constructor/destructor apis, use initalloc for complex types */
#endif // AST_H_
