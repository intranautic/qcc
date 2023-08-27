#ifndef TOKEN_H_
#define TOKEN_H_


#define TOKEN_DEBUG(tok) \
  "token {\n\tkind: %s\n\tlength: %d\n\tlocation: %p\n\tvalue: %lx\n}\n", \
  token_tostring(tok->kind), tok->length, tok->loc, tok->value

typedef struct token Token;
struct token {
  enum {
    TK_IDENTIFIER = 1,
    TK_KEYWORD,

    // literals
    TK_LCHAR,
    TK_LSTRING,
    TK_LINTEGER,
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
    TK_LNOT,

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
    TK_INC,
    TK_DEC,

    // ternary
    TK_QUESTION,

    // preprocessor
    TK_DIRECTIVE,
    TK_MACRO,
    TK_BACKSLASH,
    TK_PPNUMBER,
    TK_PPCHAR,
    TK_PPSTRING,

    TK_EOF,
    TK_NONE = 0
  } kind;
  int length;
  char* loc;
  void* value;
};

/* token constructor/destructor methods */
Token* token_create(int kind, char* loc, int length, void* value);
void token_destroy(Token* token);

/* debugging routines for token */
int token_compare(Token* token, const char* string);
const char* token_tostring(int kind);
#ifdef QCC_DEBUG
void token_dump(Token* token);
#endif // QCC_DEBUG
#endif // TOKEN_H_
