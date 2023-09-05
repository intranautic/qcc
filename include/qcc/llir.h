#ifndef LLIR_H_
#define ILIR_H_

typedef struct llir LLIR;
struct llir {
  enum {
    IR_ADD
  } op;
};

#endif // LLIR_H_
