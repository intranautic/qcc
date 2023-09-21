#include <assert.h>
#include "qcc/hashmap.h"

static const char* test_cases[] = {
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

static int test_length = sizeof(test_cases)/sizeof(*test_cases);

void test_hashmap_insert(Hashmap* hashmap) {
  for (long i = 0; i < test_length; ++i) {
    hashmap_insert(hashmap, & (Entry) {
      .key = test_cases[i],
      .value = (void *)i
    });
  }
  assert(hashmap->in_use == test_length);
  assert(hashmap->capacity == 16);

  for (long i = 0; i < test_length; ++i)
    assert(hashmap_retrieve(hashmap, test_cases[i]) == i);



  return;
}

void test_hashmap_remove(Hashmap* hashmap) {}
void test_hashmap_retrieve(Hashmap* hashmap) {}
void test_hashmap_nretrieve(Hashmap* hashmap) {}
void test_hashmap_enumerate(Hashmap* hashmap) {}

int main(void) {
  Hashmap* hashmap = hashmap_create();
  assert(hashmap != NULL);

  test_hashmap_insert(hashmap);

/*
  test_hashmap_remove(hashmap);
  test_hashmap_retreive(hashmap);
  test_hashmap_nretreive(hashmap);
  test_hashmap_enumerate(hashmap);
  test_hashmap_grow(hashmap);
*/
  hashmap_destroy(hashmap);
  return 0;
}
