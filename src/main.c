#include <stdio.h>
#include "qcc/qcc.h"

int main(int argc, const char** argv) {
  logger_init();
  logger_register("default", LOG_INFO, stderr);

  if (argv[1]) {
    Unit* unit = unit_create();
    if (unit_register(unit, argv[1]) == -1)
      return puts("could not register file");
    unit_compile(unit);
    unit_destroy(unit);
  }

  logger_destroy();
  return 0;
}
