#ifndef INITALLOC_H_
#define INITALLOC_H_
#include <stdlib.h>
#include <string.h>
// can be redefined per translation unit
// ik its hacky, idc
#define _HEAP_ALLOCATOR(sz) calloc(1, sz)
#define _MEMDUP(src, size) memcpy(_HEAP_ALLOCATOR(size), src, size)
#define INIT_ALLOC(T, ...) \
  _MEMDUP(((T []){__VA_ARGS__}), sizeof((T []){__VA_ARGS__}))
#endif // INITALLOC_H_
