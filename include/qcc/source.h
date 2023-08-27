#ifndef SOURCE_H_
#define SOURCE_H_

#include <stdio.h>
#include <stdbool.h>

typedef struct source Source;
struct source {
  bool is_ready;
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
void source_dump(Source* source);
#endif // SOURCE_H_
