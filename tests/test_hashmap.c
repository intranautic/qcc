#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include "qcc/hashmap.h"

#define TEST_STRLEN   32

static const char* test_cases[HASHMAP_DEFAULT] = {
  "PnAsOD0JdtBROoi5",
  "4bGbR2rNzxG1ZsSz",
  "z8ETfHgLnfZCLjOT",
  "hR6uHeYZkMK3e4JQ",
  "X57IEgAqD56PuQ19",
  "bTVg1RxwXq6nLFPw",
  "d5Qf9KqP5KsEcLuq",
  "16iJM4AtwFXjNpJk",
  "8QSMI97nae9PxByg",
  "6NA3kToxGMaab7Vn",
  "nQdtvZMLDgAqDEcl",
  "2BOaRnX78gBx23OZ",
  "flvxGkMFJN5NBeCt",
  "FdClUZ6ZV7fQf6h9",
  "TqWMrThaHthoVbUA",
  "hYPR5pSGxIPmPJDy"
};

int main(void) {
  int fd = open("/dev/urandom", O_RDONLY);
  char test_cases[HASHMAP_DEFAULT][TEST_STRLEN + 1] = {0};
  for (int i = 0; i < HASHMAP_DEFAULT; ++i)
    read(fd, test_cases[i], 32);

  Hashmap* hashmap = hashmap_create();
  assert(hashmap != NULL);

  // test hashmap_insert insert
  for (size_t i = 0; i < HASHMAP_DEFAULT; ++i) {
    assert(
      hashmap_insert(hashmap, & (Entry) {
        .key = test_cases[i],
        .value = (void *)i
      }) != -1
    );
  }
  assert(hashmap->in_use == HASHMAP_DEFAULT);
  assert(hashmap->capacity == HASHMAP_DEFAULT);

  // test hashmap_retrieve
  for (size_t i = 0; i < HASHMAP_DEFAULT; ++i)
    assert((size_t) hashmap_retrieve(hashmap, test_cases[i])->value == i);

  // test enumerate
  List* entry_list = hashmap_enumerate(hashmap);
  assert((size_t) list_length(entry_list) == HASHMAP_DEFAULT);
  list_destroy(entry_list);

  // test remove
  for (size_t i = 0; i < HASHMAP_DEFAULT; ++i)
    assert(hashmap_remove(hashmap, test_cases[i]) != -1);

  // test grow and operation on resized hashmap
  // hashmap should dynamically scale implicitly
  // destroy before to prevent potential collisions
  assert(
    hashmap_insert(hashmap, &(Entry){
      .key = "abcd",
      .value = NULL
    }) != -1
  );

  assert(hashmap->capacity == HASHMAP_DEFAULT<<1);
  // tombstones should be evicted during hashmap growth operation
  assert(hashmap->in_use == 1);
  // should be no memory leaks or santitization errors
  hashmap_destroy(hashmap);
  return 0;
}
