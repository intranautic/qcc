#ifndef MACRO_H_
#define MACRO_H_

#include <stdbool.h>

typedef struct macro Macro;
struct macro {
  enum {
    MACRO_OBJECT,
    MACRO_FUNCTION
  } kind;
};

#endif // MACRO_H_
