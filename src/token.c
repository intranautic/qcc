#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qcc/token.h"

Token* token_create(int kind, char* loc, int length) {
  Token* token = malloc(sizeof(Token));
  token->kind = kind;
  token->loc = loc;
  token->length = length;
  // init to null
  token->value.iliteral = 0;
  return token;
}

Token* token_construct(Token* token2) {
  Token* token = malloc(sizeof(Token));
  // shallow copy
  *token = *token2;
  return token;
}

void token_destroy(Token* token) {
  if (token) {
    switch (token->kind) {
      case TOKEN_IDENTIFIER:
        free(token->value.identifier);
        break;
      default: break;
    }
    free(token);
  }
  return;
}

int token_compare(Token* token, const char* string) {
  return strncmp(token->loc, string, token->length);
}

const char* token_tostring(int kind) {
  static const char* tok_map[] = {
    [TOKEN_IDENTIFIER] = "TOKEN_IDENTIFIER",
    [TOKEN_KEYWORD] = "TOKEN_KEYWORD",
    [TOKEN_LCHAR] = "TOKEN_LCHAR",
    [TOKEN_LSTRING] = "TOKEN_LSTRING",
    [TOKEN_LINTEGER] = "TOKEN_LINTEGER",
    [TOKEN_LFLOAT] = "TOKEN_LFLOAT",
    [TOKEN_ADD] = "TOKEN_ADD",
    [TOKEN_SUB] = "TOKEN_SUB",
    [TOKEN_MUL] = "TOKEN_MUL",
    [TOKEN_DIV] = "TOKEN_DIV",
    [TOKEN_MOD] = "TOKEN_MOD",
    [TOKEN_EQ] = "TOKEN_EQ",
    [TOKEN_NE] = "TOKEN_NE",
    [TOKEN_GT] = "TOKEN_GT",
    [TOKEN_LT] = "TOKEN_LT",
    [TOKEN_GTE] = "TOKEN_GTE",
    [TOKEN_LTE] = "TOKEN_LTE",
    [TOKEN_LAND] = "TOKEN_LAND",
    [TOKEN_LOR] = "TOKEN_LNOR",
    [TOKEN_LNOT] = "TOKEN_LNOT",
    [TOKEN_BNOT] = "TOKEN_BNOT",
    [TOKEN_BAND] = "TOKEN_BAND",
    [TOKEN_BOR] = "TOKEN_BOR",
    [TOKEN_BXOR] = "TOKEN_BXOR",
    [TOKEN_BLSHIFT] = "TOKEN_BLSHIFT",
    [TOKEN_BRSHIFT] = "TOKEN_BRSHIFT",
    [TOKEN_ASSIGN] = "TOKEN_ASSIGN",
    [TOKEN_ASGN_ADD] = "TOKEN_ASGN_ADD",
    [TOKEN_ASGN_SUB] = "TOKEN_ASGN_SUB",
    [TOKEN_ASGN_MUL] = "TOKEN_ASGN_MUL",
    [TOKEN_ASGN_DIV] = "TOKEN_ASGN_DIV",
    [TOKEN_ASGN_MOD] = "TOKEN_ASGN_MOD",
    [TOKEN_ASGN_BAND] = "TOKEN_ASGN_BAND",
    [TOKEN_ASGN_BOR] = "TOKEN_ASGN_BOR",
    [TOKEN_ASGN_BXOR] = "TOKEN_ASGN_BXOR",
    [TOKEN_ASGN_BLSHIFT] = "TOKEN_ASGN_BLSHIFT",
    [TOKEN_ASGN_BRSHIFT] = "TOKEN_ASGN_BRSHIFT",
    [TOKEN_SEMICOLON] = "TOKEN_SEMICOLON",
    [TOKEN_COLON] = "TOKEN_COLON",
    [TOKEN_COMMA] = "TOKEN_COMMA",
    [TOKEN_LPAREN] = "TOKEN_LPAREN",
    [TOKEN_RPAREN] = "TOKEN_RPAREN",
    [TOKEN_LBRACKET] = "TOKEN_LBRACKET",
    [TOKEN_RBRACKET] = "TOKEN_RBRACKET",
    [TOKEN_LBRACE] = "TOKEN_LBRACE",
    [TOKEN_RBRACE] = "TOKEN_RBRACE",
    [TOKEN_DOT] = "TOKEN_DOT",
    [TOKEN_ARROW] = "TOKEN_ARROW",
    [TOKEN_INC] = "TOKEN_INC",
    [TOKEN_DEC] = "TOKEN_DEC",
    [TOKEN_QUESTION] = "TOKEN_QUESTION",
    [TOKEN_ELLIPSIS] = "TOKEN_ELLIPSIS",
    [TOKEN_DIRECTIVE] = "TOKEN_DIRECTIVE",
    [TOKEN_MACRO] = "TOKEN_MACRO",
    [TOKEN_BACKSLASH] = "TOKEN_BACKSLASH",
    [TOKEN_PPNUMBER] = "TOKEN_PPNUMBER",
    [TOKEN_PPCHAR] = "TOKEN_PPCHAR",
    [TOKEN_PPSTRING] = "TOKEN_PPSTRING",
    [TOKEN_EOF] = "TOKEN_EOF",
    [TOKEN_NONE] = "TOKEN_NONE"
  };
  return tok_map[kind];
}

void token_dump(Token* token) {
  printf("Token {\n\tkind: %s\n\tlength: %d\n\tlocation: %p\n\tvalue: ",
    token_tostring(token->kind), token->length, token->loc);
  switch (token->kind) {
    case TOKEN_IDENTIFIER:
      printf("%s\n", token->value.identifier);
    case TOKEN_KEYWORD:
      printf("%s\n", token->value.keyword->str);
    case TOKEN_LCHAR:
      printf("%c\n", token->value.cliteral);
      break;
    case TOKEN_LSTRING:
      printf("%s\n", token->value.sliteral);
      break;
    case TOKEN_LINTEGER:
      printf("%ld\n", token->value.iliteral);
      break;
    case TOKEN_LFLOAT:
      printf("%lf\n", token->value.fliteral);
      break;
    default: puts("NONE"); break;
  };
  puts("}");
  return;
}

