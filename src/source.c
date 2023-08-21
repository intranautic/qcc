#include <stdlib.h>
#include "qcc/source.h"


Source* source_create(const char* path) {
  FILE* tmp = fopen(path, "rb");
  if (!tmp)
    return 0; //TODO: error log

  Source* source = malloc(sizeof(Source));
  source->path = path;
  source->file = tmp;
  source->contents = source->cursor = NULL;

  fseek(source->file, 0, SEEK_END);
  source->size = ftell(source->file);
  rewind(source->file);
  return source;
}

int source_fill(Source* source) {
  if (source && !source->contents) {
    source->contents = source->cursor = malloc(source->size + 1);
    return fread(source->contents, source->size, 1, source->file);
  }
  return 0;
}

void source_destroy(Source* source) {
  if (source) {
    fclose(source->file);
    free(source->contents);
    free(source);
  }
  return;
}
