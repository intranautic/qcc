#include <stdlib.h>
#include <string.h>

#include "qcc/initalloc.h"

void* allocdup(const void* src, size_t size) {
  return memcpy(malloc(size), src, size);
}
