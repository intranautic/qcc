#ifndef TOKEN_H_
#define TOKEN_H_

typedef struct token Token;
struct token {
  enum {
    TOKEN_A,
  } kind;
  int size;
  int line;
  char* loc;
  void* value;
};

#endif // TOKEN_H_
