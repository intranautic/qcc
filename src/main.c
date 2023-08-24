#include <stdio.h>
#include "qcc/qcc.h"


int main(int argc, const char** argv) {
  logger_init();
  logger_register("default", LOG_INFO, stderr);
  Hashmap* map = hashmap_create();
  hashmap_insert(map, &(Entry) {
    .key = "fucker",
    .value = (void *)1
  });
  List* l = hashmap_enumerate(map);

  list_destroy(l);
  hashmap_destroy(map);

  logger_destroy();
  return 0;
}
