#include <stdio.h>
#include <stdlib.h>

#include "qcc/ast.h"

static void depth_pad(int n) {
  for (int i = 0; i < (n*2); ++i)
    putchar(' ');
  return;
}

void ast_dump(Node* root, int depth) {
  static const char* ast_map[] = {
    [EXPR_ASSIGN] = "EXPR_ASSIGN:",
    [EXPR_BINARY] = "EXPR_BINARY:",
    [EXPR_UNARY] = "EXPR_UNARY:",
    [EXPR_POSTFIX] = "EXPR_POSTFIX:",
    [EXPR_TERNARY] = "EXPR_TERNARY:",
    [EXPR_CONST] = "EXPR_CONST:",
    [EXPR_STRING] = "EXPR_STRING:",
    [EXPR_IDENT] = "EXPR_IDENT:",
    [STMT_BLOCK] = "STMT_BLOCK:",
    [STMT_LABEL] = "STMT_LABEL:",
    [STMT_EXPR] = "STMT_EXPR:",
    [STMT_IF] = "STMT_IF:",
    [STMT_ELSE] = "STMT_ELSE:",
    [STMT_WHILE] = "STMT_WHILE:",
    [STMT_FOR] = "STMT_FOR:",
    [STMT_DO] = "STMT_DO:",
    [STMT_SWITCH] = "STMT_SWITCH:",
    [STMT_CASE] = "STMT_CASE:",
    [STMT_DEFAULT] = "STMT_DEFAULT:",
    [STMT_GOTO] = "STMT_GOTO:",
    [STMT_CONTINUE] = "STMT_CONTINUE:",
    [STMT_BREAK] = "STMT_BREAK:",
    [STMT_RETURN] = "STMT_RETURN:",
    [DECL_ENUM] = "DECL_ENUM:",
    [DECL_STRUCT] = "DECL_STRUCT:",
    [DECL_UNION] = "DECL_UNION:",
    [DECL_ARRAY] = "DECL_ARRAY:",
    [DECL_VARIABLE] = "DECL_VARIABLE:",
    [DECL_FUNCTION] = "DECL_FUNCTION:",
    [DECL_INITIALIZER] = "DECL_INITIALIZER:"
  };
  if (!root)
    return;

  depth_pad(depth);
  puts(ast_map[root->kind]);
  switch (root->kind) {
    case EXPR_IDENT:
      depth_pad(depth + 1);
      printf("name: ");
      puts(root->ident->value.ident);
      break;
    case EXPR_CONST:
      depth_pad(depth + 1);
      printf("Value: ");
      token_printlit(root->v.value);
      break;
    case EXPR_ASSIGN:
    case EXPR_BINARY:
    case EXPR_UNARY:
    case EXPR_POSTFIX:
      depth_pad(depth + 1);
      printf("Operator: %s\n", token_tostring(root->e.op->kind));
      depth_pad(depth + 1);
      if (root->e.lhs) {
        puts("Left: ");
        ast_dump(root->e.lhs, depth+2);
      } else // postfix
        putchar(10);

      depth_pad(depth + 1);
      if (root->e.rhs) {
        puts("Right: ");
        ast_dump(root->e.rhs, depth+2);
      } else // unary
        putchar(10);
      break;
    case EXPR_TERNARY:
      depth_pad(depth + 1);
      puts("Condition: ");
      if (root->c.cond)
        ast_dump(root->c.cond, depth+2);

      depth_pad(depth + 1);
      puts("Then: ");
      if (root->c.ifnode)
        ast_dump(root->c.ifnode, depth+2);

      depth_pad(depth + 1);
      puts("Else: ");
      if (root->c.elnode)
        ast_dump(root->c.elnode, depth+2);
      break;
    default: break;
  }
  return;
}

void ast_destroy(Node* root) {
  if (!root)
    return;

  switch (root->kind) {
    case EXPR_IDENT:
      if (root->ident)
        token_destroy(root->ident);
      free(root);
      break;
    case EXPR_CONST:
      if (root->v.type)
        type_destroy(root->v.type);
      if (root->v.value)
        token_destroy(root->v.value);
      free(root);
      break;
    case EXPR_ASSIGN:
    case EXPR_BINARY:
    case EXPR_UNARY:
    case EXPR_POSTFIX:
      if (root->e.op)
        token_destroy(root->e.op);
      if (root->e.lhs)
        ast_destroy(root->e.lhs);
      if (root->e.rhs)
        ast_destroy(root->e.rhs);
      free(root);
      break;
    case EXPR_TERNARY:
      if (root->c.cond)
        ast_destroy(root->c.cond);
      if (root->c.ifnode)
        ast_destroy(root->c.ifnode);
      if (root->c.elnode)
        ast_destroy(root->c.elnode);
      free(root);
      break;
    default: break;
  }
  return;
}
