#include <stdio.h>
#include <stdarg.h>

#include "qcc/ast.h"

static void depth_pad(int n) {
  for (int i = 0; i < (n*2); ++i)
    putchar(' ');
  return;
}

void ast_dump(Node* root, int depth) {
  static const char* ast_map[] = {
    [EXPR_ASSIGN] = "EXPR_ASSIGN:",
    [EXPR_CONDITION] = "EXPR_CONDITION:",
    [EXPR_BINARY] = "EXPR_BINARY:",
    [EXPR_UNARY] = "EXPR_UNARY:",
    [EXPR_POSTFIX] = "EXPR_POSTFIX:",
    [EXPR_COMMA] = "EXPR_COMMA:",
    [EXPR_SIZEOF] = "EXPR_SIZEOF:",
    [EXPR_TYPECAST] = "EXPR_TYPECAST:",
    [EXPR_CALL] = "EXPR_CALL:",
    [EXPR_SUBSCRIPT] = "EXPR_SUBSCRIPT:",
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
  depth_pad(depth);
  puts(ast_map[root->kind]);
  switch (root->kind) {
    case EXPR_CONST:
      depth_pad(depth + 1);
      printf("Value: ");
      token_printlit(root->v.value);
      break;
    case EXPR_ASSIGN:
    case EXPR_CONDITION:
    case EXPR_BINARY:
      depth_pad(depth + 1);
      printf("Operator: %s\n", token_tostring(root->e.op->kind));
      depth_pad(depth + 1);
      puts("Left: ");
      if (root->e.lhs)
        ast_dump(root->e.lhs, depth+2);
      depth_pad(depth + 1);
      puts("Right: ");
      if (root->e.rhs)
        ast_dump(root->e.rhs, depth+2);
      break;
    default: break;
  }
  return;
}


