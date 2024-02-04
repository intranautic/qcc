#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "qcc/list.h"

// default size 32, can increase as resize is costly
// 48-bit virtual addr space, prevent oom dos if buggy
// 4 page limit is good
#define HASHMAP_LIMIT   (1ul << 13)
#define HASHMAP_DEFAULT (1 << 5)
#define HASHMAP_TOMB    ((void *)(-1))

typedef unsigned long hash_t;
typedef struct entry Entry;
struct entry {
  const char* key;
  void* value;
};

typedef struct hashmap Hashmap;
struct hashmap {
  Entry* entries;
  size_t in_use;
  size_t capacity;
};

/* hashing helper function */
hash_t hashmap_hash(const char* key, size_t size);
hash_t hashmap_nhash(const char* key, size_t size, size_t length);

/* hashmap creation, deletion and designation operations */
Hashmap* hashmap_create(void);
int hashmap_grow(Hashmap* hashmap);
void hashmap_destroy(Hashmap* hashmap);

/* hashmap entry operations */
int hashmap_insert(Hashmap* hashmap, Entry* entry);
int hashmap_remove(Hashmap* hashmap, const char* key);
Entry* hashmap_retrieve(Hashmap* hashmap, const char* key);
Entry* hashmap_nretrieve(Hashmap* hashmap, const char* key, size_t length);

/* retrieves all entries present in the hashmap */
List* hashmap_enumerate(Hashmap* hashmap);
#endif // HASHMAP_H_
