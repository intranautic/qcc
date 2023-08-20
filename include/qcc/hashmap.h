#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "qcc/list.h"

#define HASHMAP_DEFAULT (1 << 4)


typedef unsigned long hash_t;
typedef struct entry Entry;
struct entry {
  const char* key;
  void* value;
};

typedef struct hashmap Hashmap;

/* hashing helper function */
hash_t hashmap_hash(const char* key, size_t size);

/* hashmap creation, deletion and designation operations */
Hashmap* hashmap_create(void);
int hashmap_grow(Hashmap* hashmap);
void hashmap_destroy(Hashmap* hashmap);

/* hashmap entry operations */
int hashmap_insert(Hashmap* hashmap, Entry* entry);
int hashmap_remove(Hashmap* hashmap, const char* key);
Entry* hashmap_retrieve(Hashmap* hashmap, const char* key);

/* retrieves all entries present in the hashmap */
List* hashmap_enumerate(Hashmap* hashmap);
#endif // HASHMAP_H_
