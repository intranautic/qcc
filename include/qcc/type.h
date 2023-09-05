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
    TYPE_CHAR     = 1,
    TYPE_SHORT    = 1 << 2,
    TYPE_INTEGER  = 1 << 3,
    TYPE_LONG     = 1 << 4,
    TYPE_FLOAT    = 1 << 5,
    TYPE_DOUBLE   = 1 << 6,
    TYPE_LDOUBLE  = 1 << 7,
    TYPE_ARRAY    = 1 << 8,
    TYPE_STRUCT   = 1 << 9,
    TYPE_UNION    = 1 << 10,
    TYPE_ENUM     = 1 << 11,
    TYPE_POINTER  = 1 << 12,
    TYPE_FUNCTION = 1 << 13,
    TYPE_VOID
  } kind;
  int align;
  int size;
  bool sign;
  union {
    struct {
      Type* array;
      int length;
    } ty_array;

    // function type
    struct {
      bool is_inline;
      bool is_local;
      bool is_variadic;
      int stacksz;
      List* params;
      List* locals;
      Type* return_type;
    } ty_func;

    // struct/union types
    struct {
      bool is_packed;
      bool is_flexible;
      bool is_anon;
      Field* fields;
    } ty_struct_or_union;

    Type* ty_pointer;
    char  ty_char;
    short ty_short;
    int   ty_int;
    long  ty_long;
    float ty_float;
    double ty_double;
    long double ty_ldouble;
    int ty_enum;
  };
};


bool type_isinteger(Type* type);
bool type_isfloat(Type* type);
bool type_isnumeric(Type* type);
Type* type_toptr(Type* type);
Type* type_fromptr(Type* type);
Type* type_array(Type* base, int length);

#endif // TYPE_H_
