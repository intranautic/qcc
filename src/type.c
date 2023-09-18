#include <stdio.h>
#include <stddef.h>
#include "qcc/type.h"
#include "qcc/initalloc.h"

// type predicates
Type* pred_void = &(Type) {.kind = TYPE_VOID, .align = 0, .size = 0};
Type* pred_bool = &(Type) {.kind = TYPE_BOOL, .align = 1, .size = 1};
Type* pred_enum = &(Type) {.kind = TYPE_ENUM, .align = 4, .size = 4};

Type* pred_char = &(Type){.kind = TYPE_CHAR, .align = 1, .size = 1, .sign = true};
Type* pred_short = &(Type){.kind = TYPE_SHORT, .align = 2, .size = 2, .sign = true};
Type* pred_int = &(Type){.kind = TYPE_INTEGER, .align = 4, .size = 4, .sign = true};
Type* pred_long = &(Type){.kind = TYPE_LONG, .align = 8, .size = 8, .sign = true};

Type* pred_uchar = &(Type) {.kind = TYPE_CHAR, .align = 1, .size = 1};
Type* pred_ushort = &(Type) {.kind = TYPE_SHORT, .align = 2, .size = 2};
Type* pred_uint = &(Type) {.kind = TYPE_INTEGER, .align = 4, .size = 4};
Type* pred_ulong = &(Type) {.kind = TYPE_LONG, .align = 8, .size = 8};

Type* pred_float = &(Type) {.kind = TYPE_FLOAT, .align = 4, .size = 4};
Type* pred_double = &(Type) {.kind = TYPE_DOUBLE, .align = 8, .size = 8};
Type* pred_ldouble = &(Type) {.kind = TYPE_LDOUBLE, .align = 8, .size = 8};

/* --- public type api --- */
void type_destroy(Type* type) {}

bool type_ischar(Type* type) { return type->kind == TYPE_CHAR; }
bool type_isshort(Type* type) { return type->kind == TYPE_SHORT; }
bool type_isint(Type* type) { return type->kind == TYPE_INTEGER; }
bool type_islong(Type* type) { return type->kind == TYPE_LONG; }
bool type_isunsign(Type* type) { return !type->sign; }
bool type_isnum(Type* type) {
  return type && (
    type_ischar(type) ||
    type_isshort(type) ||
    type_isint(type) ||
    type_islong(type)
  );
}

bool type_isptr(Type* type) { return type->kind == TYPE_POINTER; }

Type* type_ptrto(Type* type) {
  // ptr size determined by platform arch, keep 0 for now
  return (type)
    ? INIT_ALLOC(Type, {
      .kind = TYPE_POINTER,
      .size = 0,
      .align = 0,
      .ty_ptr = type
    })
    : NULL;
}

Type* type_ptrfrom(Type* type) {
  return (type_isptr(type)) ? type->ty_ptr : NULL;
}

Type* type_array(Type* type, int length) {
  return (type)
    ? INIT_ALLOC(Type, {
      .kind = TYPE_ARRAY,
      .size = type->size * length,
      .align = type->align,
      .ty_array.arrtype = type,
      .ty_array.length = length
    })
    : NULL;
}

static const char* kind_tostr(int kind) {
  if (kind & TYPE_CHAR)
    return "TYPE_CHAR";
  if (kind & TYPE_SHORT)
    return "TYPE_SHORT";
  if (kind & TYPE_INTEGER)
    return "TYPE_INTEGER";
  if (kind & TYPE_LONG)
    return "TYPE_LONG";
  if (kind & TYPE_FLOAT)
    return "TYPE_FLOAT";
  if (kind & TYPE_DOUBLE)
    return "TYPE_DOUBLE";
  if (kind & TYPE_LDOUBLE)
    return "TYPE_LDOUBLE";
  if (kind & TYPE_ARRAY)
    return "TYPE_ARRAY";
  if (kind & TYPE_STRUCT)
    return "TYPE_STRUCT";
  if (kind & TYPE_UNION)
    return "TYPE_UNION";
  if (kind & TYPE_ENUM)
    return "TYPE_ENUM";
  if (kind & TYPE_POINTER)
    return "TYPE_POINTER";
  if (kind & TYPE_FUNCTION)
    return "TYPE_FUNCTION";
  if (kind & TYPE_VOID)
    return "TYPE_VOID";
  if (kind & TYPE_BOOL)
    return "TYPE_BOOL";
  return "TYPE_ERROR";
}


void type_dump(Type* type) {
  if (!type)
    return;
  printf("Type {\n\tkind: %s\n\tsize: %d\n\talign: %d\n\tsign: %d\n}\n",
    kind_tostr(type->kind),
    type->size,
    type->align,
    type->sign
  );
  return;
}

