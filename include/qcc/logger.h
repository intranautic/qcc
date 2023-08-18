#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <fcntl.h>

#include "qcc/hashmap.h"

typedef struct sink Sink;
struct sink {
  enum {
    LEVEL_SILENT,
    LEVEL_INFO,
    LEVEL_LOG,
    LEVEL_WARNING,
    LEVEL_ERROR,
    LEVEL_FATAL
  } level;
  FILE* output;
  Sink* prev;
  Sink* next;
};

typedef struct logger Logger;

/* constructor and destructor for global logger instance */
int logger_init(void);
void logger_destroy(void);

/* helper functions for logger */
int logger_register(const char* handle, FILE* output, int level);
int logger_remove(const char* handle);
void logger_setlevel(const char* handle, int level);
int logger_getlevel(const char* handle);

/* public logging function wrappers for logger_internal */
int logger_silent(const char* fmt, ...);
int logger_info(const char* fmt, ...);
int logger_warning(const char* fmt, ...);
int logger_error(const char* fmt, ...);
int logger_fatal(const char* fmt, ...);
#endif // LOGGER_H
