#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "qcc/hashmap.h"
#include "qcc/logger.h"
#include "qcc/color.h"

#define LOG_RANGE(level) \
  (LOG_SILENT <= level && level <= LOG_FATAL)


static Logger g_logger;
static const char* g_severity[] = {
  [LOG_SILENT] = BGRN "[SILENT]:",
  [LOG_INFO] = BCYN "[INFO]:",
  [LOG_WARNING] = BYEL "[WARNING]:",
  [LOG_ERROR] = BRED "[ERROR]:",
  [LOG_FATAL] = BMAG "[FATAL]:"
};

struct sink {
  level_t level;
  FILE* output;
};

struct logger {
  Hashmap* handles;
  List* sinks;
};

static Sink* sink_create(level_t level, FILE* output) {
  Sink* sink = malloc(sizeof(Sink));
  sink->level = level;
  sink->output = output;
  return sink;
}

int logger_init(void) {
  g_logger.handles = hashmap_create();
  g_logger.sinks = list_create(0);
  return (g_logger.handles && g_logger.sinks);
}

void logger_destroy(void) {
  if (g_logger.handles)
    hashmap_destroy(g_logger.handles);
  if (g_logger.sinks) {
    for (List* tmp = g_logger.sinks; tmp->next; tmp = tmp->next)
      free(tmp->value);
    list_destroy(g_logger.sinks);
  }
  return;
}

int logger_register(const char* handle, level_t level, FILE* output) {
  if (g_logger.sinks) {
    Sink* sink = sink_create(level, output);
    hashmap_insert(g_logger.handles, & (Entry) {
      .key = handle,
      .value = (void *)sink
    });
    return list_fpush(&g_logger.sinks, sink);
  }
  return 0;
}

level_t logger_getlevel(const char* handle) {
  return (g_logger.handles && handle)
    ? ((Sink *)hashmap_retrieve(g_logger.handles, handle)->value)->level
    : -1;
}

void logger_setlevel(const char* handle, level_t level) {
  if (g_logger.handles) {
    Sink* sink =
      (Sink *)hashmap_retrieve(g_logger.handles, handle)->value;
    sink->level = level;
  }
  return;
}

static int logger_internal(level_t level, const char* fmt, va_list args) {
  Sink* sink;
  if (list_length(g_logger.sinks)) {
    for (List* entry = g_logger.sinks; entry->next; entry = entry->next) {
      sink = (Sink *)entry->value;
      if (LOG_RANGE(level) && level >= sink->level) {
        fprintf(sink->output, "%s%s ",
          g_severity[level],
          COLOR_RESET
        );
        return vfprintf(sink->output, fmt, args);
      }
    }
  }
  return 0;
}

int logger_silent(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int result = logger_internal(LOG_SILENT, fmt, args);
  va_end(args);
  return result;
}

int logger_info(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int result = logger_internal(LOG_INFO, fmt, args);
  va_end(args);
  return result;
}

int logger_warning(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int result = logger_internal(LOG_WARNING, fmt, args);
  va_end(args);
  return result;
}

int logger_error(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int result = logger_internal(LOG_ERROR, fmt, args);
  va_end(args);
  return result;
}

void logger_fatal(int error, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  logger_internal(LOG_FATAL, fmt, args);
  va_end(args);
  exit(error); // noreturn
}

