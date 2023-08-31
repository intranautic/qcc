#ifndef INITALLOC_H_
#define INITALLOC_H_

#define INIT_ALLOC(type, ...) \
  (type *)allocdup((type []){__VA_ARGS__}, sizeof(type))

void* allocdup(const void* src, size_t size);
#endif // INITALLOC_H_
