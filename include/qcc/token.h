#ifndef TOKEN_H_
#define TOKEN_H_

#include "qcc/keyword.h"


typedef struct token Token;
struct token {
  enum {
    TOKEN_IDENTIFIER = 1,
    TOKEN_KEYWORD,

    // literals
    TOKEN_LCHAR,
    TOKEN_LSTRING,
    TOKEN_LINTEGER,
    TOKEN_LFLOAT,

    // binary operators
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_MOD,

    // comparison
    TOKEN_EQ,
    TOKEN_NE,
    TOKEN_GT,
    TOKEN_LT,
    TOKEN_GTE,
    TOKEN_LTE,

    // logical
    TOKEN_LAND,
    TOKEN_LOR,
    TOKEN_LNOT,

    // bitwise
    TOKEN_BNOT,
    TOKEN_BAND,
    TOKEN_BOR,
    TOKEN_BXOR,
    TOKEN_BLSHIFT,
    TOKEN_BRSHIFT,

    // assignment
    TOKEN_ASSIGN,
    TOKEN_ASGN_ADD,
    TOKEN_ASGN_SUB,
    TOKEN_ASGN_MUL,
    TOKEN_ASGN_DIV,
    TOKEN_ASGN_MOD,
    TOKEN_ASGN_BAND,
    TOKEN_ASGN_BOR,
    TOKEN_ASGN_BXOR,
    TOKEN_ASGN_BLSHIFT,
    TOKEN_ASGN_BRSHIFT,

    // punctuators
    TOKEN_SEMICOLON,
    TOKEN_COLON,
    TOKEN_COMMA,
    // ()
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    // []
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    // {}
    TOKEN_LBRACE,
    TOKEN_RBRACE,

    // member
    TOKEN_DOT,
    TOKEN_ARROW,

    // unary operators
    TOKEN_INC,
    TOKEN_DEC,

    // ternary
    TOKEN_QUESTION,

    // ellipsis
    TOKEN_ELLIPSIS,

    // preprocessor
    TOKEN_DIRECTIVE,
    TOKEN_MACRO,
    TOKEN_BACKSLASH,
    TOKEN_PPNUMBER,
    TOKEN_PPCHAR,
    TOKEN_PPSTRING,

    TOKEN_EOF,
    TOKEN_NONE = 0
  } kind;
  int length;
  char* loc;
  union {
    char cliteral;
    char* sliteral;
    long iliteral;
    double fliteral;
    char* identifier;
    Keyword* keyword;
  } value;
};

/* token constructor/destructor methods */
Token* token_create(int kind, char* loc, int length);
Token* token_construct(Token* token);
void token_destroy(Token* token);

/* debugging routines for token */
int token_compare(Token* token, const char* string);
const char* token_tostring(int kind);
void token_dump(Token* token);
#endif // TOKEN_H_
