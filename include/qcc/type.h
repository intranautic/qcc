#ifndef TYPE_H_
#define TYPE_H_

#include <stdbool.h>

#include "qcc/list.h"
#include "qcc/token.h"

typedef struct type Type;
extern Type* pred_enum;
extern Type* pred_void;
extern Type* pred_bool;
extern Type* pred_char;
extern Type* pred_short;
extern Type* pred_int;
extern Type* pred_long;
extern Type* pred_uchar;
extern Type* pred_ushort;
extern Type* pred_uint;
extern Type* pred_ulong;
extern Type* pred_float;
extern Type* pred_double;
extern Type* pred_ldouble;

typedef struct field Field;
struct field {
  Type* type;
  Token* name;
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
    TYPE_VOID,
    TYPE_BOOL
  } kind;
  int size;
  int align;
  bool sign;
  union {
    struct {
      Type* arrtype;
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
  };
};

/* helper functions, check attributes of types */
bool type_ischar(Type* type);
bool type_isint(Type* type);
bool type_isshort(Type* type);
bool type_islong(Type* type);
bool type_isunsign(Type* type);
bool type_isnum(Type* type);

/* type constructor/destructor apis, construct from predicates */
Type* type_construct(int size, int align);
void type_destroy(Type* type);
/* reference and dereference type */
Type* type_ptrto(Type* type);
Type* type_ptrfrom(Type* type);

/* aggregate type constructors */
Type* type_array(Type* type, int length);
Type* type_struct(void);

/* function constructor */
Type* type_func(void);

/* type checking functionality, check if two types are compatible*/
int type_iseq(Type* t1, Type* t2);

/* type conversion for arithmetic, return common type */
Type* type_common(Type* t1, Type* t2);
#endif // TYPE_H_
