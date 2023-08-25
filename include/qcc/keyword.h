#ifndef KEYWORD_H_
#define KEYWORD_H_

typedef struct keyword Keyword;
struct keyword {
  enum {
    // statements
    KWRD_SWITCH,
    KWRD_CASE,
    KWRD_DEFAULT,
    KWRD_IF,
    KWRD_ELSE,
    KWRD_FOR,
    KWRD_WHILE,
    KWRD_DO,
    KWRD_CONTINUE,
    KWRD_GOTO,
    KWRD_BREAK,
    KWRD_RETURN,

    // storage class specifier
    KWRD_TYPEDEF,
    KWRD_EXTERN,
    KWRD_STATIC,
    KWRD_AUTO,
    KWRD_REGISTER,

    // type specifier
    KWRD_VOID,
    KWRD_CHAR,
    KWRD_SHORT,
    KWRD_INT,
    KWRD_LONG,
    KWRD_FLOAT,
    KWRD_DOUBLE,
    KWRD_SIGNED,
    KWRD_UNSIGNED,
    KWRD_BOOL,
    KWRD_COMPLEX,
    KWRD_IMAGINARY,

    // type qualifiers
    KWRD_CONST,
    KWRD_RESTRICT,
    KWRD_VOLATILE,

    // struct or union specifier
    KWRD_STRUCT,
    KWRD_UNION,

    // enum specifier
    KWRD_ENUM,

    // function specifier
    KWRD_INLINE,
    KWRD_END
  } kind;
  const char* str;
};

extern const Keyword* kwrd_map[KWRD_END];
#endif // KEYWORD_H_
