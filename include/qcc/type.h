#ifndef TYPE_H_
#define TYPE_H_

#include <stdbool.h>

#include "qcc/list.h"
#include "qcc/token.h"

typedef struct type Type;
typedef struct field Field;
struct field {
  Type* type;
  Token* ident;
  int index;
  int align;
  int offset;
  struct {
    int offset;
    int length;
  } bitfield;
  Field* next;
};

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
  bool sign;
  union {
    // array type
    struct {
      // type of array elements
      int kind;
      Type* next;
    } type_array;

    // function type
    struct {
      bool is_inline;
      bool is_local;
      bool is_variadic;
      int stack;
      List* params;
      Type* return_type;
    } type_func;

    // struct/union types
    struct {
      bool is_packed;
      bool is_flexible;
      bool is_anon;
      Field* fields;
    } type_struct_or_union;

    Type* type_pointer;
    char type_char;
    short type_short;
    int type_int;
    long type_long;
    float type_float;
    double type_double;
    long double type_ldouble;
    // value for enumeration, token holds identifier
    int type_enum;
  };
};
#endif // TYPE_H_
