#include <stdlib.h>
#include <string.h>

#include "qcc/hashmap.h"

#define HASHMAP_RANGE(size) (size <= (1ul << 63))

#define FNV64_OFFSET 0xcbf29ce484222325
#define FNV64_PRIME  0x100000001b3

struct hashmap {
  Entry* entries;
  size_t in_use;
  size_t capacity;
};

static hash_t fnv1a_hash64(const char* input) {
  size_t hash = FNV64_OFFSET;
  for (int i = 0; input[i]; ++i) {
    hash ^= input[i];
    hash *= FNV64_PRIME;
  }
  return hash;
}

hash_t hashmap_hash(const char* key, size_t size) {
  return fnv1a_hash64(key) % size;
}

Hashmap* hashmap_create() {
  Hashmap* hashmap = malloc(sizeof(Hashmap));
  hashmap->entries = malloc(HASHMAP_DEFAULT * sizeof(Entry));
  hashmap->in_use = 0;
  hashmap->capacity = HASHMAP_DEFAULT;
  return hashmap;
}

int hashmap_grow(Hashmap* hashmap) {
  Hashmap tmp;
  if (!hashmap)
    return -1;

  if (HASHMAP_RANGE(hashmap->capacity << 1)) {
    tmp = *hashmap;
    hashmap->capacity <<= 1;
    hashmap->entries = calloc(1, hashmap->capacity * sizeof(Entry));
    // rehash each entry by insert
    for (size_t i = 0; i < tmp.capacity; ++i) {
      if (tmp.entries[i].key)
        hashmap_insert(hashmap, &tmp.entries[i]);
    }
    // reset hashmap in_use, since insert api increments
    hashmap->in_use = tmp.in_use;
    // destroy old map
    free(tmp.entries);
    return hashmap->capacity; // return new capacity if ok
  }
  // not within hashmap range, return -1 error
  return -1;
}

void hashmap_destroy(Hashmap* hashmap) {
  if (hashmap) {
    free(hashmap->entries);
    free(hashmap);
  }
  return;
}

int hashmap_insert(Hashmap* hashmap, Entry* entry) {
  hash_t index;
  if (hashmap) {
    // if hashmap is full, grow
    if (hashmap->in_use >= hashmap->capacity) {
      if (hashmap_grow(hashmap) == -1)
        return -1; // cannot grow hashmap
    }

    index = hashmap_hash(entry->key, hashmap->capacity);
    while (hashmap->entries[index].key) {
      if (!strcmp(hashmap->entries[index].key, entry->key))
        return index; // already exists
      index = ++index % hashmap->capacity;
    }
    // shallow copy, dont hold ref to object since could be stack alloc'd
    hashmap->entries[index].key = entry->key;
    hashmap->entries[index].value = entry->value;
    hashmap->in_use++;
  }
  return -1;
}

int hashmap_remove(Hashmap* hashmap, const char* key) {
  hash_t index;
  if (hashmap) {
    index = hashmap_hash(key, hashmap->capacity);
    while (hashmap->entries[index].key) {
      if (!strcmp(hashmap->entries[index].key, key)) {
        hashmap->entries[index] = (Entry){
          .key = 0,
          .value = 0
        };
        hashmap->in_use--;
        return index;
      }
      index = ++index % hashmap->capacity;
    }
  }
  return -1;
}

Entry* hashmap_retrieve(Hashmap* hashmap, const char* key) {
  hash_t index;
  if (hashmap) {
    index = hashmap_hash(key, hashmap->capacity);
    while (hashmap->entries[index].key) {
      if (!strcmp(hashmap->entries[index].key, key))
        return &hashmap->entries[index];
      index = ++index % hashmap->capacity;
    }
  }
  // null for error, no entry found
  return 0;
}

