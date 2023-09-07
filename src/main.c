#include <stdio.h>
#include "qcc/qcc.h"

int main(int argc, const char** argv) {
  logger_init();
  logger_register("default", LOG_INFO, stderr);

  if (argv[1]) {
    Unit* unit = unit_create(argv[1]);
    unit_compile(unit);
  }

  logger_destroy();
  return 0;
}
