#ifndef TOKEN_H_
#define TOKEN_H_


typedef struct token Token;
struct token {
  enum {
    A
  } kind;
  char* loc;
  int size;
};

#endif // TOKEN_H_
