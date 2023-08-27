#include <stdlib.h>
#include <string.h>

#include "qcc/token.h"

Token* token_create(int kind, char* loc, int length, void* value) {
  Token* token = malloc(sizeof(Token));
  token->kind = kind;
  token->loc = loc;
  token->length = length;
  token->value = value;
  return token;
}

void token_destroy(Token* token) {
  if (token)
    free(token);
  return;
}

int token_compare(Token* token, const char* string) {
  return strncmp(token->loc, string, token->length);
}

const char* token_tostring(int kind) {
  static const char* tok_map[] = {
    [TK_IDENTIFIER] = "TK_IDENTIFIER",
    [TK_KEYWORD] = "TK_KEYWORD",
    [TK_LCHAR] = "TK_LCHAR",
    [TK_LSTRING] = "TK_LSTRING",
    [TK_LINTEGER] = "TK_LINTEGER",
    [TK_LFLOAT] = "TK_LFLOAT",
    [TK_ADD] = "TK_ADD",
    [TK_SUB] = "TK_SUB",
    [TK_MUL] = "TK_MUL",
    [TK_DIV] = "TK_DIV",
    [TK_MOD] = "TK_MOD",
    [TK_EQ] = "TK_EQ",
    [TK_NE] = "TK_NE",
    [TK_GT] = "TK_GT",
    [TK_LT] = "TK_LT",
    [TK_GTE] = "TK_GTE",
    [TK_LTE] = "TK_LTE",
    [TK_LAND] = "TK_LAND",
    [TK_LOR] = "TK_LNOR",
    [TK_LNOT] = "TK_LNOT",
    [TK_BNOT] = "TK_BNOT",
    [TK_BAND] = "TK_BAND",
    [TK_BOR] = "TK_BOR",
    [TK_BXOR] = "TK_BXOR",
    [TK_BLSHIFT] = "TK_BLSHIFT",
    [TK_BRSHIFT] = "TK_BRSHIFT",
    [TK_ASSIGN] = "TK_ASSIGN",
    [TK_ASGN_ADD] = "TK_ASGN_ADD",
    [TK_ASGN_SUB] = "TK_ASGN_SUB",
    [TK_ASGN_MUL] = "TK_ASGN_MUL",
    [TK_ASGN_DIV] = "TK_ASGN_DIV",
    [TK_ASGN_MOD] = "TK_ASGN_MOD",
    [TK_ASGN_BAND] = "TK_ASGN_BAND",
    [TK_ASGN_BOR] = "TK_ASGN_BOR",
    [TK_ASGN_BXOR] = "TK_ASGN_BXOR",
    [TK_ASGN_BLSHIFT] = "TK_ASGN_BLSHIFT",
    [TK_ASGN_BRSHIFT] = "TK_ASGN_BRSHIFT",
    [TK_SEMICOLON] = "TK_SEMICOLON",
    [TK_COLON] = "TK_COLON",
    [TK_COMMA] = "TK_COMMA",
    [TK_LPAREN] = "TK_LPAREN",
    [TK_RPAREN] = "TK_RPAREN",
    [TK_LBRACKET] = "TK_LBRACKET",
    [TK_RBRACKET] = "TK_RBRACKET",
    [TK_LBRACE] = "TK_LBRACE",
    [TK_RBRACE] = "TK_RBRACE",
    [TK_DOT] = "TK_DOT",
    [TK_ARROW] = "TK_ARROW",
    [TK_INC] = "TK_INC",
    [TK_DEC] = "TK_DEC",
    [TK_QUESTION] = "TK_QUESTION",
    [TK_DIRECTIVE] = "TK_DIRECTIVE",
    [TK_MACRO] = "TK_MACRO",
    [TK_BACKSLASH] = "TK_BACKSLASH",
    [TK_PPNUMBER] = "TK_PPNUMBER",
    [TK_PPCHAR] = "TK_PPCHAR",
    [TK_PPSTRING] = "TK_PPSTRING",
    [TK_EOF] = "TK_EOF",
    [TK_NONE] = "TK_NONE"
  };
  return tok_map[kind];
}
