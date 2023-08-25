#ifndef TOKEN_H_
#define TOKEN_H_

typedef struct token Token;
struct token {
  enum {
    TK_IDENTIFIER,
    TK_TYPE,
    TK_KEYWORD,

    // literals
    TK_LCHAR,
    TK_LSTRING,
    TK_LNUMBER,
    TK_LFLOAT,

    // binary operators
    TK_ADD,
    TK_SUB,
    TK_MUL,
    TK_DIV,
    TK_MOD,

    // comparison
    TK_EQ,
    TK_NE,
    TK_GT,
    TK_LT,
    TK_GTE,
    TK_LTE,

    // logical
    TK_LAND,
    TK_LOR,
    TL_LNOT,

    // bitwise
    TK_BNOT,
    TK_BAND,
    TK_BOR,
    TK_BXOR,
    TK_BLSHIFT,
    TK_BRSHIFT,

    // assignment
    TK_ASSIGN,
    TK_ASGN_ADD,
    TK_ASGN_SUB,
    TK_ASGN_MUL,
    TK_ASGN_DIV,
    TK_ASGN_MOD,
    TK_ASGN_BAND,
    TK_ASGN_BOR,
    TK_ASGN_BXOR,
    TK_ASGN_BLSHIFT,
    TK_ASGN_BRSHIFT,

    // punctuators
    TK_SEMICOLON,
    TK_COLON,
    TK_COMMA,
    TK_STAR,
    // ()
    TK_LPAREN,
    TK_RPAREN,
    // []
    TK_LBRACKET,
    TK_RBRACKET,
    // {}
    TK_LBRACE,
    TK_RBRACE,

    // member
    TK_DOT,
    TK_ARROW,

    // unary operators
    TK_UADD,
    TK_USUB,
    TK_INC,
    TK_DEC,
    TK_ADDR,

    // ternary
    TK_QUESTION,

    // preprocessor
    TK_DIRECTIVE,
    TK_BACKSLASH,
    TK_PPNUMBER,
    TK_PPCHAR,
    TK_PPSTRING,

    TK_EOF,
    TK_NONE = -1
  } kind;
  int length;
  char* loc;
  void* value;
};

/* token constructor/destructor methods */
Token* token_create(int kind, char* loc, int length, void* value);
void token_destroy(Token* token);

/* compare token with a string */
int token_compare(Token* token, const char* string);
#ifdef QCC_DEBUG
void token_dump(Token* token);
#endif // QCC_DEBUG
#endif // TOKEN_H_
