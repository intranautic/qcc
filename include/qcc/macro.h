#ifndef MACRO_H_
#define MACRO_H_

typedef struct macro Macro;
struct macro {
  enum {
    MACRO_OBJECT,
    MACRO_FUNCTION
  } kind;
  List* tokens;
  List* params;
};

Macro* macro_create(void);
void macro_destroy(Macro* macro);
#endif // MACRO_H_
