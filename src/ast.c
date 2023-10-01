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
    [NODE_ASSIGN] = "NODE_ASSIGN:",
    [NODE_BINARY] = "NODE_BINARY:",
    [NODE_UNARY] = "NODE_UNARY:",
    [NODE_POSTFIX] = "NODE_POSTFIX:",
    [NODE_CALL] = "NODE_CALL:",
    [NODE_TYPECAST] = "NODE_TYPECAST",
    [NODE_TERNARY] = "NODE_TERNARY:",
    [NODE_CONST] = "NODE_CONST:",
    [NODE_STRING] = "NODE_STRING:",
    [NODE_IDENT] = "NODE_IDENT:",
    [NODE_BLOCK] = "NODE_BLOCK:",
    [NODE_LABEL] = "NODE_LABEL:",
    [NODE_EXPR] = "NODE_EXPR:",
    [NODE_IF] = "NODE_IF:",
    [NODE_ELSE] = "NODE_ELSE:",
    [NODE_WHILE] = "NODE_WHILE:",
    [NODE_FOR] = "NODE_FOR:",
    [NODE_DO] = "NODE_DO:",
    [NODE_SWITCH] = "NODE_SWITCH:",
    [NODE_CASE] = "NODE_CASE:",
    [NODE_DEFAULT] = "NODE_DEFAULT:",
    [NODE_GOTO] = "NODE_GOTO:",
    [NODE_CONTINUE] = "NODE_CONTINUE:",
    [NODE_BREAK] = "NODE_BREAK:",
    [NODE_RETURN] = "NODE_RETURN:",
    [NODE_ENUM] = "NODE_ENUM:",
    [NODE_STRUCT] = "NODE_STRUCT:",
    [NODE_UNION] = "NODE_UNION:",
    [NODE_ARRAY] = "NODE_ARRAY:",
    [NODE_VARIABLE] = "NODE_VARIABLE:",
    [NODE_FUNCTION] = "NODE_FUNCTION:",
    [NODE_INITIALIZER] = "NODE_INITIALIZER:"
  };
  if (!root)
    return;

  depth_pad(depth);
  puts(ast_map[root->kind]);
  switch (root->kind) {
    case NODE_IDENT:
      depth_pad(depth + 1);
      printf("name: ");
      puts(root->symbol->ident->value.ident);
      break;
    case NODE_CONST:
      depth_pad(depth + 1);
      printf("Value: ");
      token_printlit(root->v.value);
      break;
    case NODE_ASSIGN:
    case NODE_BINARY:
    case NODE_UNARY:
    case NODE_POSTFIX:
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
    case NODE_IF:
    case NODE_TERNARY:
      if (root->c.cond) {
        depth_pad(depth + 1);
        puts("Condition: ");
        ast_dump(root->c.cond, depth+2);
      }

      if (root->c.ifnode) {
        depth_pad(depth + 1);
        puts("Then: ");
        ast_dump(root->c.ifnode, depth+2);
      }

      if (root->c.elnode) {
        depth_pad(depth + 1);
        puts("Else: ");
        ast_dump(root->c.elnode, depth+2);
      }
      break;
    case NODE_CALL:
      depth_pad(depth + 1);
      puts("Name: ");
      if (root->f.name)
        ast_dump(root->f.name, depth+2);

      depth_pad(depth + 1);
      puts("Arguments: ");
      if (root->f.args)
        ast_dump(root->f.args, depth+2);
      break;
    default: break;
  }
  return;
}

void ast_destroy(Node* root) {
  if (!root)
    return;

  switch (root->kind) {
    case NODE_IDENT:
      if (root->symbol)
        symbol_destroy(root->symbol);;
      free(root);
      break;
    case NODE_CONST:
      if (root->v.type)
        type_destroy(root->v.type);
      if (root->v.value)
        token_destroy(root->v.value);
      free(root);
      break;
    case NODE_ASSIGN:
    case NODE_BINARY:
    case NODE_UNARY:
    case NODE_POSTFIX:
      if (root->e.op)
        token_destroy(root->e.op);
      if (root->e.lhs)
        ast_destroy(root->e.lhs);
      if (root->e.rhs)
        ast_destroy(root->e.rhs);
      free(root);
      break;
    case NODE_TERNARY:
      if (root->c.cond)
        ast_destroy(root->c.cond);
      if (root->c.ifnode)
        ast_destroy(root->c.ifnode);
      if (root->c.elnode)
        ast_destroy(root->c.elnode);
      free(root);
      break;
    case NODE_CALL:
      if (root->f.name)
        ast_destroy(root->f.name);
      if (root->f.args)
        ast_destroy(root->f.args);
      break;
    default: break;
  }
  return;
}

void ast_fold(Node* root) {
  if (!root)
    return;


  return;
}
