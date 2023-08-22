#include <stdio.h>
#include "qcc/qcc.h"


int main(int argc, const char** argv) {
  logger_init();
  logger_register("default", LOG_INFO, stderr);
  logger_info("fuck you man\n");
  logger_destroy();
  return 0;
}
