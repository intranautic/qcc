#ifndef TYPE_H_
#define TYPE_H_

#include <stdbool.h>

typedef struct type Type;
struct type {
  enum {
    TYPE_CHAR,
    TYPE_SHORT,
    TYPE_INTEGER,
    TYPE_LONG,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_LDOUBLE,
    TYPE_ARRAY,
    TYPE_STRUCT,
    TYPE_UNION,
    TYPE_ENUM,
    TYPE_POINTER,
    TYPE_FUNCTION,
    TYPE_VOID
  } kind;
  int align;
  int size;
  bool is_unsigned;
  union {
  };
};

#endif // TYPE_H_
