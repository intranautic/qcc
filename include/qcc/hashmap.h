#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define HASHMAP_DEFAULT (1 << 4)

typedef unsigned long hash_t;
typedef struct entry {
  const char* key;
  void* value;
} entry_t;

typedef struct hashmap {
  entry_t* entries;
  size_t in_use;
  size_t capacity;
} hashmap_t;

/* hashing helper function */
hash_t hashmap_hash(const char* key, size_t size);

/* hashmap creation, deletion and designation operations */
hashmap_t* hashmap_create();
int hashmap_grow(hashmap_t* hashmap);
void hashmap_destroy(hashmap_t* hashmap);

/* hashmap entry operations */
int hashmap_insert(hashmap_t* hashmap, entry_t* entry);
int hashmap_remove(hashmap_t* hashmap, const char* key);
entry_t* hashmap_retrieve(hashmap_t* hashmap, const char* key);

void entry_dump(entry_t* entry);
#endif // HASHMAP_H_
