#include <stdlib.h>
#include <string.h>

#include "qcc/hashmap.h"

#define HASHMAP_THRESH(size) (size <= HASHMAP_LIMIT)
#define HASHMAP_DEAD(key) (key == HASHMAP_TOMB)
#define HASHMAP_ACTIVE(key) (key && !HASHMAP_DEAD(key))

#define FNV64_OFFSET 0xcbf29ce484222325
#define FNV64_PRIME  0x100000001b3

static hash_t fnv1a_hash64(const char* input, size_t length) {
  size_t hash = FNV64_OFFSET;
  for (int i = 0; i < length; ++i) {
    hash ^= input[i];
    hash *= FNV64_PRIME;
  }
  return hash;
}

hash_t hashmap_hash(const char* key, size_t size) {
  return fnv1a_hash64(key, strlen(key)) % size;
}

hash_t hashmap_nhash(const char* key, size_t size, size_t length) {
  return fnv1a_hash64(key, length) % size;
}

Hashmap* hashmap_create() {
  Hashmap* hashmap = malloc(sizeof(Hashmap));
  hashmap->entries = calloc(1, HASHMAP_DEFAULT * sizeof(Entry));
  hashmap->in_use = 0;
  hashmap->capacity = HASHMAP_DEFAULT;
  return hashmap;
}

int hashmap_grow(Hashmap* hashmap) {
  if (!hashmap)
    return -1;

  if (HASHMAP_THRESH(hashmap->capacity << 1)) {
    Hashmap tmp = *hashmap;
    hashmap->in_use = 0;
    hashmap->capacity <<= 1; 
    hashmap->entries = calloc(1, hashmap->capacity * sizeof(Entry));
    // rehash each entry by insert
    for (size_t i = 0; i < tmp.capacity; ++i) {
      if (HASHMAP_ACTIVE(tmp.entries[i].key)) 
        hashmap_insert(hashmap, &tmp.entries[i]);
    }
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
      // if not tombstone and equal
      if (!HASHMAP_DEAD(hashmap->entries[index].key)) {
        if (!strcmp(hashmap->entries[index].key, entry->key))
          return index; // already exists 
      }
      index = (index + 1) % hashmap->capacity;
    }
    // shallow copy, dont hold ref to object since could be stack alloc'd
    hashmap->entries[index] = *entry;
    hashmap->in_use++;
    return index;
  }
  return -1;
}

int hashmap_remove(Hashmap* hashmap, const char* key) {
  hash_t index;
  if (hashmap) {
    index = hashmap_hash(key, hashmap->capacity);
    while (hashmap->entries[index].key) {
      // if not tombstone and equal
      // tomb evicts entry slot, does not decrement size
      // required for correct dynamic resizing
      if (!HASHMAP_DEAD(hashmap->entries[index].key)) {
        if (!strcmp(hashmap->entries[index].key, key)) {
          hashmap->entries[index] = (Entry) {
            .key = HASHMAP_TOMB,
            .value = 0
          };
          return index;
        } 
      }
      index = (index + 1) % hashmap->capacity;
    }
  }
  return -1;
}

Entry* hashmap_retrieve(Hashmap* hashmap, const char* key) {
  return hashmap_nretrieve(hashmap, key, strlen(key));
}

Entry* hashmap_nretrieve(Hashmap* hashmap, const char* key, size_t length) {
  hash_t index;
  if (hashmap) {
    index = hashmap_nhash(key, hashmap->capacity, length);
    while (hashmap->entries[index].key) {
      if (!HASHMAP_DEAD(hashmap->entries[index].key)) {
        if (!strncmp(hashmap->entries[index].key, key, length))
          return &hashmap->entries[index];
      }
      index = (index + 1) % hashmap->capacity;
    }
  }
  // null for error, no entry found
  return 0;
}

// order is not guaranteed, performs linear scan over hashmap
List* hashmap_enumerate(Hashmap* hashmap) {
  List* entries;
  if (hashmap && hashmap->in_use > 0) {
    entries = NULL;

    for (size_t i = 0; i < hashmap->capacity; ++i) {
      if (HASHMAP_ACTIVE(hashmap->entries[i].key))
        list_fpush(&entries, &hashmap->entries[i]);
    }
    return entries;
  }
  return 0;
}

