#ifndef LOGGER_H_
#define LOGGER_H_


typedef struct sink Sink;
typedef struct logger Logger;
typedef enum {
  LOG_SILENT,
  LOG_INFO,
  LOG_WARNING,
  LOG_ERROR,
  LOG_FATAL
} level_t;

/* logger constructor and destructor methods */
int logger_init(void);
void logger_destroy(void);

/* register new sink to logger */
int logger_register(const char* handle, level_t level, FILE* output);

/* set/get levels of logger */
level_t logger_getlevel(const char* handle);
void logger_setlevel(const char* handle, level_t level);

/* exposed logging api wrappers around logger_internal */
int logger_silent(const char* fmt, ...);
int logger_info(const char* fmt, ...);
int logger_warning(const char* fmt, ...);
int logger_error(const char* fmt, ...);
void logger_fatal(int error, const char* fmt, ...);
#endif // LOGGER_H_

