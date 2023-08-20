#include <stdlib.h>

#include "qcc/logger.h"
#include "qcc/color.h"


struct logger {
  Hashmap* sinks;
  Sink* head;
  struct tm* time;
};

static Logger g_logger;

static const char* g_severity[] = {
  [LEVEL_SILENT] = BGRN "[SILENT]",
  [LEVEL_INFO] = BCYN "[INFO]",
  [LEVEL_WARNING] = BYEL "[WARNING]",
  [LEVEL_ERROR] = BRED "[ERROR]",
  [LEVEL_FATAL] = BMAG "[FATAL]"
};

static Sink* sink_create(int level, FILE* output, Sink* next, Sink* prev) {
  Sink* sink = malloc(sizeof(Sink));
  sink->level = level;
  sink->output = output;
  sink->next = next;
  sink->prev = prev;
  return sink;
}

int logger_init(void) {
  g_logger.sinks = hashmap_create();
  g_logger.head = sink_create(LEVEL_INFO, stderr, 0, 0);
  return (g_logger.sinks && g_logger.head);
}

void logger_destroy(void) {
  if (g_logger.sinks)
    hashmap_destroy(g_logger.sinks);

  if (g_logger.head) {
    Sink* tmp = g_logger.head;
    for (Sink* next; tmp; tmp = next) {
      next = tmp->next;
      free(tmp);
    }
  }
  return;
}

int logger_register(const char* handle, FILE* output, int level) {}

int logger_remove(const char* handle) {}

void logger_setlevel(const char* handle, int level) {
  // sanity check since were querying hashmap, need valid key
  if (handle) {
    Sink* sink = hashmap_retrieve(g_logger.sinks, handle)->value;
    sink->level = level;
  }
  return;
}

int logger_getlevel(const char* handle) {
  if (handle) {
    Sink* sink = hashmap_retrieve(g_logger.sinks, handle)->value;
    if (sink)
      return sink->level;
  }
  return -1;
}

static int logger_internal(int level, const char* fmt, ...) {}

int logger_silent(const char* fmt, ...) {}
int logger_info(const char* fmt, ...) {}
int logger_warning(const char* fmt, ...) {}
int logger_error(const char* fmt, ...) {}
int logger_fatal(const char* fmt, ...) {}

