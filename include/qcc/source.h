#ifndef SOURCE_H_
#define SOURCE_H_

#include <stdio.h>

typedef struct source Source;
struct source {
  FILE* file;
  const char* path;
  char* contents;
  char* cursor;
  size_t size;
  int line;
};

/* constructor/fill/destructor methods for source, fairly simple */
Source* source_create(const char* path);
int source_fill(Source* source);
void source_destroy(Source* source);
#endif // SOURCE_H_
